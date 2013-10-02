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
 * Include composer autoload
 */
require_once(SITE_ROOT . '/vendor/autoload.php');

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
 * Include po parser for Language class
 */
require_once(SITE_ROOT . '/core/includes/poparser.php');

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
