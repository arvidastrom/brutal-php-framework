<?php
// Enable disable development mode
define('DEVELOPMENT', true);

// Database settings
define('DB_HOST', 'localhost');
define('DB_NAME', 'database');
define('DB_USER', 'root');
define('DB_PASS', '');
define('DB_LOG', true);

// Enable/disable instagram sdk
define('INSTAGRAM', true);
define('INSTAGRAM_CLIENT_ID', '');
define('INSTAGRAM_CLIENT_SECRET', '');
define('INSTAGRAM_REDIRECT_URI', '');

// Enable/disable facebook sdk
define('FACEBOOK', true);
define('FACEBOOK_APP_ID', '');
define('FACEBOOK_APP_SECRET', '');
define('FACEBOOK_LOCALE', 'sv_SE');

// Administrator password, hashed using sha1('pass');
define('ADMIN_PASSWORD', '058ab13948b09cc2bd2510881bb32c59ee96c401');

// Session prefix, used to namespace the site to avoid collisions with other sites on the server
define('SESSION_PREFIX', 'session_prefix');

define('SITE_TITLE', 'Site title');
