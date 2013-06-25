<?php
// Enable disable development mode
define('DEVELOPMENT', true);

// Enable/disable facebook sdk
define('FACEBOOK', true);
define('FACEBOOK_APP_ID', '');
define('FACEBOOK_APP_SECRET', '');
define('FACEBOOK_LOCALE', 'sv_SE');

// Session prefix, used to namespace the site to avoid collisions with other sites on the server
define('SESSION_PREFIX', 'session_prefix');

// The title of the site
define('SITE_TITLE', 'Website Title');

// Database settings
$databases = array(
	'default' => array(
		'host' => 'localhost',
		'name' => 'databasename',
		'user' => 'root',
		'pass' => '',
	),
);

// default theme
$theme = 'brutal';
