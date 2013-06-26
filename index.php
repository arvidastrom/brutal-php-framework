<?php
// start the session
session_start();

// Strip magic quotes from request data.
if (function_exists('get_magic_quotes_gpc') && get_magic_quotes_gpc()) {
	// Create lamba style unescaping function (for portability)
	$quotes_sybase = strtolower(ini_get('magic_quotes_sybase'));
	$unescape_function = (empty($quotes_sybase) || $quotes_sybase === 'off') ? 'stripslashes($value)' : 'str_replace("\'\'","\'",$value)';
	$stripslashes_deep = create_function('&$value, $fn', '
		if (is_string($value)) {
			$value = ' . $unescape_function . ';
		}
		else if (is_array($value)) {
			foreach ($value as &$v) $fn($v, $fn);
		}
	');

	// Unescape data
	$stripslashes_deep($_POST, $stripslashes_deep);
	$stripslashes_deep($_GET, $stripslashes_deep);
	$stripslashes_deep($_COOKIE, $stripslashes_deep);
	$stripslashes_deep($_REQUEST, $stripslashes_deep);
}

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

// normal page view, prints html and page template files wrapped around content
define('PAGE_CALLBACK_NORMAL', 0);

// ajax page view, prints the returned data as json encoded without html and page template
define('PAGE_CALLBACK_AJAX', 1);

// blank page view, only prints the content from the url callback
define('PAGE_CALLBACK_BLANK', 2);

// includes classes
require_once(SITE_ROOT . '/core/includes/class.url.inc');
require_once(SITE_ROOT . '/core/includes/class.session.inc');
require_once(SITE_ROOT . '/core/includes/class.database.inc');
require_once(SITE_ROOT . '/core/includes/class.template.inc');
require_once(SITE_ROOT . '/core/includes/class.core.inc');

// external libraries
require_once(SITE_ROOT . '/core/libraries/RedBeanPHP3_4_7/rb.php');
require_once(SITE_ROOT . '/core/libraries/Swift-5.0.1/lib/swift_required.php');

// configure redbean
foreach ($databases as $dbkey => $dbinfo) {
	if ($dbkey == 'default') {
		R::setup('mysql:host=' . $dbinfo['host'] . ';dbname=' . $dbinfo['name'], $dbinfo['user'], $dbinfo['pass']);

		// dont change the database after deployment
		if (!DEVELOPMENT) {
			R::freeze(true);
		}
	}
	else {
		R::addDatabase($dbkey, 'mysql:host=' . $dbinfo['host'] . ';dbname=' . $dbinfo['name'], $dbinfo['user'], $dbinfo['pass'], true);
	}
}

// include modules
if (FACEBOOK) {
	require_once(SITE_ROOT . '/core/modules/facebook/facebook.inc');
}

// include the app file
require_once(SITE_ROOT . '/app/app.inc');

Core::init();
Core::view();
