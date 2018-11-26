#ifndef __COMMAND_HOOK_HPP__
#define __COMMAND_HOOK_HPP__

#include <string>

#include <Command.hpp>

#include <mesos/hook.hpp>
#include <mesos/module/hook.hpp>

#include <stout/option.hpp>

namespace criteo {
namespace mesos {

/**
 * Hook calling external commands to handle hook events.
 *
 * The external command is executed in a fork so that it does not threaten
 * the Mesos agent. The hook is also protected from infinite loop by killing
 * the child process after a certain amount of time if it does not exit.
 */
class CommandHook : public ::mesos::Hook {
 public:
  /*
   * Constructor
   * Each argument can be empty. In that case the corresponding hook
   * method will not be treated.
   *
   * @param runTaskLabelCommand The command used by
   *   slaveRunTaskLabelDecorator method if provided.
   * @param executorEnvironmentCommand The command used by
   *   slaveExecutorEnvironmentDecorator if provided.
   * @param removeExecutorCommand The command used by
   *   slaveRemoveExecutorHook if provided.
   * @param isDebugMode If true, logs inputs and outputs of the commands,
   *   otherwise logs nothing
   */
  explicit CommandHook(const Option<Command> &runTaskLabelCommand,
                       const Option<Command> &executorEnvironmentCommand,
                       const Option<Command> &removeExecutorCommand,
                       bool isDebugMode = false);

  virtual ~CommandHook() {}

  /*
   * Run an external command computing a list of labels to add to the executor.
   * The command will be provided with a JSON containing all the
   * information required to compute the list of labels. The output of the
   * command is a JSON that is parsed into mesos::Labels.
   *
   * @param taskInfo The information regarding the tasks.
   * @param executorInfo The information regarding the executor.
   * @param frameworkInfo The information regarding the framework.
   * @param slaveInfo The information regarding the slave.
   * @return The labels to add to the executor.
   */
  virtual Result<::mesos::Labels> slaveRunTaskLabelDecorator(
      const ::mesos::TaskInfo &taskInfo,
      const ::mesos::ExecutorInfo &executorInfo,
      const ::mesos::FrameworkInfo &frameworkInfo,
      const ::mesos::SlaveInfo &slaveInfo) override;

  /*
   * Run an external command producing a list of environment variables to
   * add to the executor. The command is provided with a JSON containing
   * all the information required to compute the list of environment
   * variables. The output of the command is a JSON that is parsed into
   * mesos::Evironment.
   *
   * @param executorInfo The information regarding the executor.
   * @return The environment variables to add to the executor.
   */
  virtual Result<::mesos::Environment> slaveExecutorEnvironmentDecorator(
      const ::mesos::ExecutorInfo &executorInfo) override;

  /*
   * Run an external command when an executor is removed from the agent.
   * The command is provided with a JSON containing all the information
   * relative to the framework and the executor.
   *
   * @param frameworkInfo The information regarding the framework.
   * @param executorInfo The information regarding the executor.
   */
  virtual Try<Nothing> slaveRemoveExecutorHook(
      const ::mesos::FrameworkInfo &frameworkInfo,
      const ::mesos::ExecutorInfo &executorInfo) override;

  inline const Option<Command> &runTaskLabelCommand() const {
    return m_runTaskLabelCommand;
  }

  inline const Option<Command> &executorEnvironmentCommand() const {
    return m_executorEnvironmentCommand;
  }

  inline const Option<Command> &removeExecutorCommand() const {
    return m_removeExecutorCommand;
  }

 private:
  Option<Command> m_runTaskLabelCommand;
  Option<Command> m_executorEnvironmentCommand;
  Option<Command> m_removeExecutorCommand;
  bool m_isDebugMode;
};
}  // namespace mesos
}  // namespace criteo

#endif
