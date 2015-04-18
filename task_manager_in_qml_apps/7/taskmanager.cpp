/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <QDebug>
#include <stdexcept>
#include "taskmanager.h"

/* TaskManager::Task */

TaskManager::Task::Task(Scenario & theScenario, QObject * theObject, const QString & theMethodName, 
			const QVariantList & theArguments, const GeneratedValueNames & theGeneratedValueNames):
	scenario_(theScenario),
	object_(theObject),
	methodName_(theMethodName),
	args_(theArguments),
	generatedValueNames_(theGeneratedValueNames) {

	if(!object_)
		throw std::runtime_error("Object is not specified");
	else if(methodName_.isEmpty())
		throw std::runtime_error("Method name is not specified");
	else if(args_.size() > ArgumentsCount)
		throw std::runtime_error(QString("Can only accept %1 arguments but %2 given").arg(ArgumentsCount).arg(theArguments.size()).toStdString());
	else if(generatedValueNames_.contains(QString()))
		throw std::runtime_error("There is an empty name among generated value names");
}

QSharedPointer<TaskManager::Task> TaskManager::Task::addNextTask(QObject * theObject, const QString & theMethodName, 
								 const QVariantList & theArguments, const GeneratedValueNames & theGeneratedValueNames) {
 	if(successSuccessor_)
		throw std::runtime_error(QString("Success successor for task %1 already exists: %2").arg(getDescription()).arg(successSuccessor_->getDescription()).toStdString());

	QSharedPointer<Task> task(new Task(scenario_, theObject, theMethodName, theArguments, theGeneratedValueNames));
	successSuccessor_.swap(task);
	qDebug() << QString("Success successor %1 added for task %2").arg(successSuccessor_->getDescription()).arg(getDescription());
	failureSuccessor_ = successSuccessor_;
	return successSuccessor_;
}

TaskManager::Task::Branch TaskManager::Task::addNextTaskBranch() {
 	if(successSuccessor_)
		throw std::runtime_error(QString("Success successor for task %1 already exists: %2").arg(getDescription()).arg(successSuccessor_->getDescription()).toStdString());
	else if(failureSuccessor_)
		throw std::runtime_error(QString("Failure successor for task %1 already exists: %2").arg(getDescription()).arg(failureSuccessor_->getDescription()).toStdString());

	QSharedPointer<Task> sa(new Task(scenario_, &scenario_.getTaskManager(), scenario_.getTaskManager().getDummyTaskName()));
	QSharedPointer<Task> fa(new Task(scenario_, &scenario_.getTaskManager(), scenario_.getTaskManager().getDummyTaskName()));
	successSuccessor_.swap(sa);
	failureSuccessor_.swap(fa);
	qDebug() << QString("Success successor %1 and failure successor %2 added for task %3").arg(successSuccessor_->getDescription()).arg(failureSuccessor_->getDescription()).arg(getDescription());
	Branch branch = { successSuccessor_, failureSuccessor_ };
	return branch;
}

static QString extractUnqualifiedClassName(const QString & theQualifiedClassName) {
	const int p = theQualifiedClassName.lastIndexOf("::");
	return p > -1 ? theQualifiedClassName.mid(p + 2) : theQualifiedClassName;
}

QString TaskManager::Task::getDescription() const {
	Q_ASSERT(object_);
	const QMetaObject * meta = object_->metaObject();
	const QString className = meta ? extractUnqualifiedClassName(meta->className()) : "<unknown>";
	return QString("\"%1::%2\"").arg(className).arg(methodName_);
}

/* TaskManager::Scenario */

TaskManager::Scenario::Scenario(TaskManager & theTaskManager, const QString & theName): taskManager_(theTaskManager), name_(theName) {
	if(name_.isEmpty())
		throw std::runtime_error("Scenario name is not specified");

	QSharedPointer<Task> dummyTask(new Task(*this, &taskManager_, taskManager_.getDummyTaskName()));
	rootTask_.swap(dummyTask);
}

const QString & TaskManager::Scenario::getName() const {
	return name_;
}

QSharedPointer<TaskManager::Task> TaskManager::Scenario::getRootTask() const {
	return rootTask_.dynamicCast<Task>();
}

TaskManager & TaskManager::Scenario::getTaskManager() {
	return taskManager_;
}

/* TaskManager */

TaskManager::TaskManager() {
	if(false || 
	   !QObject::connect(&nextTaskTimer_, SIGNAL(timeout()), this, SLOT(onNextTaskTimer())) ||
	   false)
		throw std::runtime_error("Failed to connect task manager internals");
}

QSharedPointer<TaskManager::Scenario> TaskManager::addScenario(const QString & theName) {
	QSharedPointer<Scenario> scenario(new Scenario(*this, theName));
	scenarios_.enqueue(scenario);
	restartNextTaskTimer();
	return scenario;
}

void TaskManager::invokeProcessing() {
	processTask();
}

void TaskManager::onNextTaskTimer() {
	try {
		processTask();
	} catch(const std::exception & theE) {
		qDebug() << "Failed to process task:" << theE.what();
	}
}

QString TaskManager::getDummyTaskName() {
	return "__dummy__";
}

void TaskManager::processTask() {
	struct NestingLevelGuard {
		NestingLevelGuard(int & theLevel): level(theLevel) { ++level; };
		~NestingLevelGuard() { --level; };
		int & level;
	} nestingLevelGuard(nestingLevel_);

	if(nestingLevel_ > 1)
		throw std::runtime_error(QString("Cannot process task since another task is being run (nesting level %1)").arg(nestingLevel_).toStdString());

	if(scenarios_.isEmpty())
		return;

	ScenarioPtr scenario = scenarios_.head();
	Q_ASSERT(scenario);
	qDebug() << "Working with scenario" << scenario->getName();
	TaskPtr task;

	while(true) {
		task = scenario->rootTask_;

		if(task && task->methodName_ == getDummyTaskName()) {
			scenario->rootTask_ = task->successSuccessor_;
			qDebug() << QString("Dummy task %1 skipped").arg(task->getDescription());
		}
		else break;
	}

	if(task = scenario->rootTask_) {
		const TaskResult tr = callTaskHandler(task);

		if(tr.type == TaskResult::MetaError) {
			const QString metaErrorMessage = QString("Failed to invoke task %1").arg(task->getDescription());
			qFatal("%s", metaErrorMessage.toLocal8Bit().data());
			return;
		}

		switch(tr.type) {
		case TaskResult::Success: 
			scenario->rootTask_ = task->successSuccessor_;
			break;
		case TaskResult::NeedToWait: 
			// just leave current task active
			return;
		case TaskResult::MetaError:
			// this must be handled earlier
			qFatal("logic error");
			return;
		default:
			// Treat everything else as a failure
			scenario->rootTask_ = task->failureSuccessor_;
			break;
		}
	}

	if(scenario->rootTask_) {
		qDebug() << QString("Next task set to %1").arg(scenario->rootTask_->getDescription());
	}
	else {
		qDebug() << "Scenario is finished";
		scenarios_.dequeue();
	}	
	
	restartNextTaskTimer();
}

TaskManager::TaskResult TaskManager::callTaskHandler(const TaskPtr & theTask) {
	Q_ASSERT(theTask);
	Q_ASSERT(Task::ArgumentsCount > 9);
	TaskResult rv = TaskResult::MetaError;
	const int argsSize = theTask->args_.size();
	Q_ASSERT(argsSize <= Task::ArgumentsCount);
	const bool isCalled = QMetaObject::invokeMethod(theTask->object_, 
							theTask->methodName_.toLocal8Bit().data(), 
							Qt::DirectConnection, 
							Q_RETURN_ARG(TaskManager::TaskResult, rv),
							argsSize > 0 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[0])) : QGenericArgument(0),
							argsSize > 1 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[1])) : QGenericArgument(0),
							argsSize > 2 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[2])) : QGenericArgument(0),
							argsSize > 3 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[3])) : QGenericArgument(0),
							argsSize > 4 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[4])) : QGenericArgument(0),
							argsSize > 5 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[5])) : QGenericArgument(0),
							argsSize > 6 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[6])) : QGenericArgument(0),
							argsSize > 7 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[7])) : QGenericArgument(0),
							argsSize > 8 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[8])) : QGenericArgument(0),
							argsSize > 9 ? Q_ARG(QVariant, prepareTaskArgument(theTask, theTask->args_[9])) : QGenericArgument(0));

	if(!isCalled) 
		return TaskResult::MetaError;

	for(QVariantHash::const_iterator i = rv.generatedValues.constBegin(), e = rv.generatedValues.constEnd(); i != e; ++i) {
		GeneratedValueNames::const_iterator j = theTask->generatedValueNames_.constFind(i.key());

		if(j == theTask->generatedValueNames_.constEnd()) {
			const QString metaErrorMessage = QString("Generated value \"%1\" is not registered for task \"%2\"").arg(i.key()).arg(theTask->getDescription());
			qFatal("%s", metaErrorMessage.toLocal8Bit().data());
		}
		
		theTask->scenario_.dynArgs_[i.key()] = *i;
		qDebug() << QString("Generated value \"%1\" saved, data: %2").arg(i.key()).arg(i->canConvert<QString>() ? i->toString() : QString("<not representible>"));
	}

	return rv;
}

QVariant TaskManager::prepareTaskArgument(const TaskPtr & theTask, const QVariant & theRawData) const {
	if(theRawData.canConvert<TaskManager::DynArg>()) {
		const DynArg da = theRawData.value<TaskManager::DynArg>();
		QVariantHash::const_iterator i = theTask->scenario_.dynArgs_.constFind(da.name);
		return i != theTask->scenario_.dynArgs_.constEnd() ? *i : QVariant();
	}

	return theRawData;
}

void TaskManager::restartNextTaskTimer() {
	nextTaskTimer_.stop();
	nextTaskTimer_.setSingleShot(true);
	nextTaskTimer_.setInterval(0);
	nextTaskTimer_.start();
}
