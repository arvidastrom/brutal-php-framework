<?php
class Variable {
	private static $variables = array();
	private static $not_set_variables = array();
	private static $db_installed = false;

	static function set($name, $value) {
		self::check_db();

		$pdo = Database::get();

		// check if we already have this variable stored in the database
		$sth = $pdo->prepare('SELECT COUNT(*) as `match` FROM brutal_variables WHERE name = :name');
		$sth->execute(array(
			':name' => $name,
		));
		$match = $sth->fetchObject()->match;

		// update the row
		if ($match == 1) {
			$sth = $pdo->prepare('UPDATE brutal_variables SET value = :value WHERE name = :name');
			$sth->execute(array(
				':name' => $name,
				':value' => $value,
			));
		}
		// insert a new row to the database
		else {
			$sth = $pdo->prepare('INSERT INTO brutal_variables (value, name) VALUES (:value, :name)');
			$sth->execute(array(
				':name' => $name,
				':value' => $value,
			));
		}

		self::$variables[$name] = $value;

		if (array_key_exists($name, self::$not_set_variables)) {
			unset(self::$not_set_variables[$name]);
		}
	}

	static function get($name, $default = null) {
		self::check_db();

		if (array_key_exists($name, self::$not_set_variables)) {
			return $default;
		}

		if (array_key_exists($name, self::$variables)) {
			return self::$variables[$name];
		}

		else {
			$pdo = Database::get();

			// check if this variable is set
			$sth = $pdo->prepare('SELECT value FROM brutal_variables WHERE name = :name');
			$sth->execute(array(
				':name' => $name,
			));
			$match = $sth->fetchObject();

			if ($match) {
				self::$variables[$name] = $match->value;
				return $match->value;
			}
		}

		self::$not_set_variables[$name] = true;

		return $default;
	}

	static function del($name) {
		self::check_db();

		if (!array_key_exists($name, self::$not_set_variables)) {
			if (array_key_exists($name, self::$variables)) {
				unset(self::$variables[$name]);
			}

			$sth = Database::get()->prepare('DELETE FROM brutal_variables WHERE name = :name');

			$sth->exec(array(
				':name' => $name,
			));
			
			self::$not_set_variables[$name] = true;
		}
	}

	private static function check_db() {
		// check if this function was called before
		if (self::$db_installed) {
			return true;
		}

		global $databases;

		$sth = Database::get()->prepare('SELECT table_name FROM information_schema.tables WHERE table_schema = :database AND table_name = :table');

		$sth->execute(array(
			':database' => $databases['default']['name'],
			':table' => 'brutal_variables',
		));

		$table = $sth->fetchObject();

		if ($table == false) {
			$query = '
				CREATE TABLE IF NOT EXISTS `brutal_variables` (
					`name` varchar(255) NOT NULL,
					`value` TEXT NOT NULL,
				PRIMARY KEY (`name`))
			';

			Database::get()->exec($query);
		}

		self::$db_installed = true;

		return true;
	}
}
