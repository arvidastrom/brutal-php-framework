<?php
session_start();
header('P3P: CP="CAO PSA OUR"');

// define the root directory of the site (this files directory)
define('SITE_ROOT', dirname(__FILE__));


// include configuration
require_once(SITE_ROOT . '/app/config.php');


// Show all errors on development mode
if (DEVELOPMENT) {
	error_reporting(E_ALL);
}
else {
	error_reporting(0);
}


// template directory
define('TEMPLATE_DIR', SITE_ROOT . '/app/templates');

// upload directory
define('UPLOAD_DIR', SITE_ROOT . '/app/uploads');

// normal page view, prints html and page template files wrapped around content
define('PAGE_CALLBACK_NORMAL', 0);

// ajax page view, prints the returned data as json encoded without html and page template
define('PAGE_CALLBACK_AJAX', 1);

// blank page view, only prints the content from the url callback
define('PAGE_CALLBACK_BLANK', 2);


// includes classes
require_once(SITE_ROOT . '/core/inc/class.url.inc');
require_once(SITE_ROOT . '/core/inc/class.variable.inc');
require_once(SITE_ROOT . '/core/inc/class.core.inc');
require_once(SITE_ROOT . '/core/inc/class.template.inc');

// include libraries
require_once(SITE_ROOT . '/core/libs/redbeanphp/rb.php');

// include modules
if (FACEBOOK) {
	require_once(SITE_ROOT . '/core/modules/facebook/facebook.inc');
}

// include the app class
require_once(SITE_ROOT . '/app/app.inc');

// setup database connection
R::setup('mysql:host='. DB_HOST .';dbname='. DB_NAME, DB_USER, DB_PASS);

if (DEVELOPMENT) {
	$queryLogger = RedBean_Plugin_QueryLogger::getInstanceAndAttach(R::getDatabaseAdapter());
}
else {
	R::freeze(true);
}

Core::init();

if (DEVELOPMENT) {
	Core::add_html_variables(array('query_logs' => $queryLogger->getLogs()));
}

Core::view();