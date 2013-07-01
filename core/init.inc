<?php
/**
 * Start the session.
 *
 * Always start to session to be able to use the session class.
 */
session_start();

/**
 * Fix magic quotes.
 *
 * Cleans up post, get, cookie and request global variables
 * from the ugliness put on by magic quotes.
 */
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

/**
 * P3P Compact Policy (CP) headers.
 *
 * Send P3P Compact Policy (CP) headers to prevent IE from blocking cookies
 */
header('P3P: CP="CAO PSA OUR"');


/**
 * Site root constant.
 *
 * Defines the root of the framework, one level up from this file.
 */
define('SITE_ROOT', dirname(dirname(__FILE__)));

/**
 * Include configuration file.
 */
require_once(SITE_ROOT . '/app/config.inc');

/**
 * Error reporting.
 *
 * Turns on all error reporting when developing and
 * keeps it off on production.
 */
if (!empty($development)) {
	error_reporting(E_ALL);

	if (!empty($pretty_errors)) {
		/**
		 * Include advanced error reporting features
		 */
		include_once(SITE_ROOT . '/core/libraries/Whoops/Exception/ErrorException.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Exception/Frame.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Exception/FrameCollection.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Exception/Inspector.php');

		include_once(SITE_ROOT . '/core/libraries/Whoops/Handler/HandlerInterface.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Handler/Handler.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Handler/CallbackHandler.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Handler/JsonResponseHandler.php');
		include_once(SITE_ROOT . '/core/libraries/Whoops/Handler/PrettyPageHandler.php');

		include_once(SITE_ROOT . '/core/libraries/Whoops/Run.php');

		$run = new Whoops\Run;
		$handler = new Whoops\Handler\PrettyPageHandler;
		$run->pushHandler($handler);
		$run->register();
	}
}
else {
	error_reporting(0);
}

/**
 * Includes all core static classes.
 */
require_once(SITE_ROOT . '/core/includes/class.url.inc');
require_once(SITE_ROOT . '/core/includes/class.session.inc');
require_once(SITE_ROOT . '/core/includes/class.variable.inc');
require_once(SITE_ROOT . '/core/includes/class.database.inc');
require_once(SITE_ROOT . '/core/includes/class.template.inc');
require_once(SITE_ROOT . '/core/includes/class.brutal.inc');

/**
 * Include the app "module" file.
 */
require_once(SITE_ROOT . '/app/app.inc');

/**
 * Includes all external libraries.
 *
 * @todo: Possibly move these into each own separate module, they might not always be needed.
 */
require_once(SITE_ROOT . '/core/libraries/RedBeanPHP3_4_7/rb.php');
require_once(SITE_ROOT . '/core/libraries/Swift-5.0.1/lib/swift_required.php');

/**
 * Configure redbeanphp.
 *
 * Configures all databases to redbeanphp.
 *
 * @todo: Possibly move this to a separate module and use the Database class in the Variable class.
 */
foreach ($databases as $dbkey => $dbinfo) {
	// setup redbeanphp with the default db
	if ($dbkey == 'default') {
		R::setup('mysql:host=' . $dbinfo['host'] . ';dbname=' . $dbinfo['name'], $dbinfo['user'], $dbinfo['pass']);

		// dont change the database after deployment
		if (empty($development)) {
			R::freeze(true);
		}
	}
	// add additional databases
	else {
		R::addDatabase($dbkey, 'mysql:host=' . $dbinfo['host'] . ';dbname=' . $dbinfo['name'], $dbinfo['user'], $dbinfo['pass'], true);
	}
}
