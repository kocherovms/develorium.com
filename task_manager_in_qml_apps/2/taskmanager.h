/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QVariant>
#include <QSharedPointer>

class TaskManager: public QObject {
	Q_OBJECT
public:
	enum TaskResult { 
		MetaError,
		Success, 
		Failure
	};
	class Scenario;
	class Task {
	public:
		struct Branch {
			QSharedPointer<Task> successTask;
			QSharedPointer<Task> failureTask;
		};
	public:
		Task(Scenario & theScenario, QObject * theObject, const QString & theMethodName, const QVariantList & theArguments = QVariantList());
		QSharedPointer<Task> addNextTask(QObject * theObject, const QString & theMethodName, const QVariantList & theArguments);
		Branch addNextTaskBranch();
		QString getDescription() const;
	public:
		Scenario & scenario_;
		QObject * object_ = 0;
		QString methodName_;
		enum { ArgumentsCount = 10 };
		QVariantList args;
		QSharedPointer<Task> successAncestor_;
		QSharedPointer<Task> failureAncestor_;
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
	};
	typedef QSharedPointer<Scenario> ScenarioPtr;
public:
	TaskManager();
	QSharedPointer<Scenario> addScenario(const QString & theName);
private slots:
	void onNextTaskTimer();
private:
	static QString getDummyTaskName();
	void processTask();
	TaskResult callTaskHandler(const TaskPtr & theTask);
	void restartNextTaskTimer();
private:
	typedef QQueue<ScenarioPtr> Scenarios;
	Scenarios scenarios_;
	int nestingLevel_ = 0;
	QTimer nextTaskTimer_;
};

#endif
