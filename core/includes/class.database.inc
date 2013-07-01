<?php
 /**
  * Database class.
  *
  * Sets up PDO objects connected to specified databases.
  * 
  * @package Core
  */
class Database {
	/**
	 * Stores all connected database objects.
	 *
	 * @access private
	 *
	 * @var array
	 */
	private static $connections = array();

	/**
	 * Adds a new database connection.
	 *
	 * Example:
	 *
	 * <code>
	 * Database::add('db2', array(
	 *  	'type' => 'mysql',
	 *  	'host' => 'localhost',
	 *  	'name' => 'dbname',
	 *  	'user' => 'dbuser',
	 *  	'pass' => 'dbpass',
	 * ));
	 * </code>
	 *
	 * @param string $key          The key which identifies this connection.
	 * @param array  $server_info  Server details.
	 */
	static function add($key, $server_info) {
		global $databases;

		$databases[$key] = $server_info;
	}

	/**
	 * Get a database connect.
	 *
	 * Checks if we already have a connection for the specificied server and returns that object.
	 * It will automatically connect if there is no active connection.
	 * 
	 * Examples:
	 *
	 * * Get the default database connection.
	 *
	 * <code>
	 * $db = Database::get();
	 * </code>
	 * 
	 * * Get a additional database connection.
	 *
	 * <code>
	 * $sth = Database::get('db2')->prepare('SELECT name FROM page WHERE id = :id');
	 * $result = $sth->execute(array(':id' => 5));
	 * </code>
	 *
	 * @param string $key  The key which identifies this connection.
	 *
	 * @return PDO A pdo object connected to the speficied database.
	 */
	static function get($key = 'default') {
		if (!array_key_exists($key, self::$connections)) {
			self::connect($key);
		}

		return self::$connections[$key];
	}

	/**
	 * Connects to a database.
	 *
	 * * Checks if there is any database settings for the specified key.
	 * * Configure pdo settings to use UTF-8 and turns on exception error mode on development.
	 *
	 * @access private
	 *
	 * @param string $key The database key defined in the app config.
	 */
	private static function connect($key) {
		global $databases, $development;

		if (!array_key_exists($key, $databases)) {
			throw new Exception('No database settings for ' . $key);
		}

		$server_info = $databases[$key];

		$db = new PDO('mysql:host=' . $server_info['host'] .';dbname='. $server_info['name'], $server_info['user'], $server_info['pass'], array(PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8'));
		$db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_OBJ);

		if ($development) {
			$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		}

		self::$connections[$key] = $db;
	}
}
