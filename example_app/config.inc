<?php
// The title of the site
$site_title = 'Brutal PHP Framework';

// Database settings
$databases = array(
	'default' => array(
		'host' => 'localhost',
		'name' => 'brutal',
		'user' => 'root',
		'pass' => '',
	),
);

// Enable/disable development mode
$development = true;

// Enabled/disable pretty errors
$pretty_errors = true;

// Enabled modules
$modules = array(
	'facebook',
	'instagram',
);

// Session prefix, used to namespace the site to avoid collisions with other sites on the server
$session_prefix = 'brutal';

// Default theme
$theme = 'brutal';
