<?php
/**
 * Form_genomic_browser class file
 *
 * PHP version 7
 *
 * @category LORIS_Module
 * @package  Genomics
 * @author   Christine Rogers <rogers@bic.mni.mcgill.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://github.com/aces/Loris
*/
namespace LORIS\genomic_browser;

/**
 * Vie_Genomic_File Class
 *
 * This class is a Form to present Genomic_file data
 *
 * @category LORIS_Module
 * @package  Genomics
 * @author   Christine Rogers <rogers@bic.mni.mcgill.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://github.com/aces/Loris
*/

class View_Genomic_File extends \NDB_Form
{
    /**
     * Variables to store the DCCID, genomic file ID
    */
    var $candID;
    var $genomic_file_ID;

    /**
    * Determine whether the user has permission to view this page
    *
    * @return bool whether the user has access
    */
    function _hasAccess()
    {
        $user =& \User::singleton();

        // Add site control? See Imaging Browser example
        return ($user->hasPermission('genomic_browser_view_allsites') ||
                $user->hasPermission('genomic_browser_view_site'));
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

        $this->DB = \Database::singleton();

        $this->genomic_file_id = $_REQUEST['GenomicFileID'];
        $this->candID          = $_REQUEST['candID'];

        if (!empty($this->candID) || !empty($this->genomic_file_id) ) {
            $this->_setFileData();
            $this->tpl_data['has_permission'] = ($this->_hasAccess()) ? true : false;
        }
    }

    /**
    * Sets values for Genomic data files
    *
    * @param string $category type of data: raw,cleaned,gwas
    *
    * @return NULL
    */
    function _setFileData($category = null)
    {
        if (empty($this->genomic_file_id)) {
            $files = $this->DB->pselect(
                "SELECT g.GenomicFileID
                FROM genomic_candidate_files_rel g WHERE g.CandID=:cid ",
                array('cid' => $this->candID)
            );
        } else {
            $files = array(
                      array('GenomicFileID' => $this->genomic_file_id),
                     );
        }
        $this->tpl_data['files'] = array();
        foreach ($files as $fileRow) {
            $FileObj = new Genomic_File($fileRow['GenomicFileID']);

            // Get a few parameters
            $paramFilename         = basename($FileObj->getParameter('FileName'));
            $paramNormalization    = $FileObj->getParameter('Normalization');
            $paramAnalysisProtocol = $FileObj->getParameter('AnalysisProtocol');
            $paramFileInsertDate   = $FileObj->getParameter('Date_inserted');
            $paramQCDate           = $FileObj->getParameter('QCLastChangeTime');

            // load $file array
            $file = array(
                     'FileID'           => $fileRow['GenomicFileID'],
                     'Filename'         => $paramFileName,
                     'FullFilename'     => $FileObj->getParameter('FileName'),
                     'FilePackage'      => $FileObj->getParameter('FilePackage'),
                     'FileType'         => $FileObj->getParameter('FileType'),
                     'FileSize'         => $FileObj->getParameter('FileSize'),
                     'Description'      => $FileObj->getParameter('Description'),
                     'Category'         => $FileObj->getParameter('Category'),
                     'Pipeline'         => $FileObj->getParameter('Pipeline'),
                     'Algorithm'        => $FileObj->getParameter('Algorithm'),
                     'Normalization'    => $paramNormalization,
                     'SampleID'         => $FileObj->getParameter('SampleID'),
                     'Caveat'           => $FileObj->getParameter('Caveat'),
                     'Notes'            => $FileObj->getParameter('Notes'),
                     'AnalysisProtocol' => $paramAnalysisProtocol,
                     'AnalysisModality' => $FileObj->getParameter(
                         'AnalysisModality'
                     ),
                     'AnalysisDate'     => $this->_getDate(
                         $FileObj,
                         'Date_taken',
                         $analysisDate
                     ),
                     'SampleDate'       => $this->_getDate(
                         $FileObj,
                         'Date_sample',
                         $sampleDate
                     ),
                     'FileInsertDate'   => $paramFileInsertDate,
                     'Selected'         => $FileObj->getParameter('Selected'),
                     'QCStatus'         => $FileObj->getParameter('QCStatus'),
                     'QCDate'           => $paramQCDate,
                    );

            $this->tpl_data['files'][] = $file;
        }
    }
    /**
    * Gets the date according to the type
    *
    * @param GenomicFile $file  file object
    * @param string      $type  type of the date
    * @param array       $array array containing the date
    *
    * @return date if exists, if not an empty string
    */
    function _getDate($file, $type, $array)
    {
        $pattern  = "/(\d{4})-?(\d{2})-?(\d{2})/";
        $fileType = $file->getParameter($type);
        if (preg_match($pattern, $fileType, $array)) {
            return (mktime(12, 0, 0, $array[2], $array[3], $array[1]));
        } else {
            return "";
        }
    }

}
?>
