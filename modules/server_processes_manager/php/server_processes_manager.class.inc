<?php
/**
 * Page used to view/manage the processes launched asynchronously by LORIS users.
 *
 * PHP Version 5
 *
 * @category ServerProcessLauncher
 * @package  Loris
 * @author   Nicolas Brossard <nicolasbrossard.mni@gmail.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://github.com/aces/Loris
 */
namespace LORIS\server_processes_manager;

/**
 * Builds a new filter to view/manage processes launched asynchronously by LORIS
 * users.
 *
 * PHP Version 5
 *
 * @category ServerProcessLauncher
 * @package  Loris
 * @author   Nicolas Brossard <nicolasbrossard.mni@gmail.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://github.com/aces/Loris
 */
class Server_Processes_Manager extends \NDB_Menu_Filter
{
    /**
     * Determines whether the current user has access to the page.
     *
     * @note   overloaded function
     * @return bool true if the user has access, false otherwise
     * @access private
     */
    function _hasAccess()
    {
        $user = \User::singleton();
        return $user->hasPermission('server_processes_manager');
    }

    /**
     * Sets up the filter and result table layout.
     *
     * @return bool true if the set up is successful, false otherwise.
     */
    function _setupVariables()
    {
        // This will update all process informations in the database
        // Must execute this before loading the form
        $serverProcessesMonitor = new ServerProcessesMonitor();
        $serverProcessesMonitor->getProcessesState();

        // Columns displayed in the result table
        $this->columns = array(
                          'pid',
                          'type',
                          'stdout_file',
                          'stderr_file',
                          'exit_code_file',
                          'exit_code',
                          'userid',
                          'start_time',
                          'end_time',
                          'exit_text',
                         );

        // Allow to sort on pid, type and userid
        $this->validFilters = array(
                               'pid',
                               'type',
                               'userid',
                              );

        $this->query = " FROM server_processes 
                         WHERE 1=1";

        $this->formToFilter = array(
                               'pid'    => 'pid',
                               'type'   => 'type',
                               'userid' => 'userid',
                              );

        return true;
    }


    /**
     * Does the setup required for this page. By default, sets up elements
     * that are common to every type of page. May be overridden by a specific
     * page or specific page type.
     *
     * @param string $name       The test name being accessed
     * @param string $page       The subtest being accessed (may be null)
     * @param string $identifier The identifier for the data to load on this page
     * @param string $commentID  The CommentID to load the data for
     * @param string $formname   The name to give this form
     *
     * @return none
     */
    function _setupPage($name, $page, $identifier, $commentID, $formname)
    {
        parent::_setupPage($name, $page, $identifier, $commentID, $formname);

        $this->addBasicText('pid', 'PID:');
        $this->addBasicText('type', 'Type:');
        $this->addBasicText('userid', 'UserId:');

        return true;
    }
    /**
     * Gathers JS dependecies and merge them with the parent
     *
     * @return array of javascript to be inserted
     */
    function getJSDependencies()
    {
        $factory = \NDB_Factory::singleton();
        $baseURL = $factory->settings()->getBaseURL();
        $deps    = parent::getJSDependencies();
        return array_merge(
            $deps,
            array($baseURL . "/server_processes_manager/js/columnFormatter.js")
        );
    }

}
?>
