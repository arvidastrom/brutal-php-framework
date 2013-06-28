<?php
class Database {
	// store all active connections
	private static $connections = array();

	// add a server config during runtime
	static function add($key, $server_info) {
		global $databases;

		$databases[$key] = $server_info;
	}

	// get a database object
	static function get($server = 'default') {
		if (!array_key_exists($server, self::$connections)) {
			self::connect($server);
		}

		return self::$connections[$server];
	}

	// private function to conncet to a database
	private static function connect($server) {
		global $databases, $development;

		if (!array_key_exists($server, $databases)) {
			throw new Exception('No database settings for ' . $server);
		}

		$server_info = $databases[$server];

		$db = new PDO('mysql:host=' . $server_info['host'] .';dbname='. $server_info['name'], $server_info['user'], $server_info['pass'], array(PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8'));
		$db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_OBJ);

		if ($development) {
			$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		}

		self::$connections[$server] = $db;
	}
}
