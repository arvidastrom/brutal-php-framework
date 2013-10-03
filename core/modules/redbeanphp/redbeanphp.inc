<?php
class R extends RedBean_Facade {};

global $databases, $development;

/**
 * Configure redbeanphp.
 *
 * Configures all databases to redbeanphp.
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
