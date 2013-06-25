<?php
// start the session
session_start();

// send P3P Compact Policy (CP) headers to prevent IE from blocking cookies
header('P3P: CP="CAO PSA OUR"');

// define the root directory of the site (this file's directory)
define('SITE_ROOT', dirname(__FILE__));

// include configuration
require_once(SITE_ROOT . '/app/config.inc');

// Show all errors on development mode
if (DEVELOPMENT) {
	error_reporting(E_ALL);
}
else {
	error_reporting(0);
}

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
require_once(SITE_ROOT . '/core/inc/class.session.inc');
require_once(SITE_ROOT . '/core/inc/class.database.inc');
require_once(SITE_ROOT . '/core/inc/class.template.inc');
require_once(SITE_ROOT . '/core/inc/class.core.inc');

// include modules
if (FACEBOOK) {
	require_once(SITE_ROOT . '/core/modules/facebook/facebook.inc');
}

// include the app file
require_once(SITE_ROOT . '/app/app.inc');

Core::init();
Core::view();
