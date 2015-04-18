/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <QObject>
#include <QQueue>
#include <QSet>
#include <QTimer>
#include <QVariant>
#include <QVariantHash>
#include <QSharedPointer>

class TaskManager: public QObject {
	Q_OBJECT
public:
	struct DynArg {
		DynArg() {}
		DynArg(const QString & theName): name(theName) {}
		operator QVariant() const {
			QVariant rv;
			rv.setValue(*this);
			return rv;
		}
		QString name;
	};
	struct TaskResult {
		enum Type {
			MetaError,
			Success,
			Failure,
			NeedToWait
		};
		TaskResult(const Type theType, const QVariantHash & theGeneratedValues = QVariantHash()): type(theType), generatedValues(theGeneratedValues) {
		}
		Type type = MetaError;
		QVariantHash generatedValues;
	};
	typedef QSet<QString> GeneratedValueNames;
	class Scenario;
	class Task {
	public:
		struct Branch {
			QSharedPointer<Task> successTask;
			QSharedPointer<Task> failureTask;
		};
	public:
		Task(Scenario & theScenario, QObject * theObject, 
		     const QString & theMethodName, 
		     const QVariantList & theArguments = QVariantList(), 
		     const GeneratedValueNames & theGeneratedValueNames = GeneratedValueNames());
		QSharedPointer<Task> addNextTask(QObject * theObject, 
						 const QString & theMethodName, 
						 const QVariantList & theArguments = QVariantList(),
						 const GeneratedValueNames & theGeneratedValueNames = GeneratedValueNames());
		Branch addNextTaskBranch();
		QString getDescription() const;
	public:
		Scenario & scenario_;
		QObject * object_ = 0;
		QString methodName_;
		enum { ArgumentsCount = 10 };
		QVariantList args_;
		QSharedPointer<Task> successSuccessor_;
		QSharedPointer<Task> failureSuccessor_;
		GeneratedValueNames generatedValueNames_;
	};
	typedef QSharedPointer<Task> TaskPtr;
	class Scenario {
	public:
		Scenario(TaskManager & theTaskManager, const QString & theName);
		const QString & getName() const;
		QSharedPointer<Task> getRootTask() const;
		TaskManager & getTaskManager();
	public:
		TaskManager & taskManager_;
		QString name_;
		TaskPtr rootTask_;
		QVariantHash dynArgs_;
	};
	typedef QSharedPointer<Scenario> ScenarioPtr;
public:
	TaskManager();
	QSharedPointer<Scenario> addScenario(const QString & theName);
	Q_INVOKABLE void invokeProcessing();
private slots:
	void onNextTaskTimer();
private:
	static QString getDummyTaskName();
	void processTask();
	TaskResult callTaskHandler(const TaskPtr & theTask);
	QVariant prepareTaskArgument(const TaskPtr & theTask, const QVariant & theRawData) const;
	void restartNextTaskTimer();
private:
	typedef QQueue<ScenarioPtr> Scenarios;
	Scenarios scenarios_;
	int nestingLevel_ = 0;
	QTimer nextTaskTimer_;
};

Q_DECLARE_METATYPE(TaskManager::DynArg);

#endif
