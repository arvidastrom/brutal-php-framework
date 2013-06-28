<?php
class Variable {
	private static $_variables = array();
	private static $_not_set_variables = array();

	static function set($key, $value) {
		if (R::count('variables')) {
			$variable = R::findOrDispense('variables', '`key` = :key LIMIT 1', array(':key' => $key));
			$variable = array_shift($variable);
		}
		else {
			$variable = R::dispense('variables');
		}

		$variable->key = $key;
		$variable->value = $value;

		self::$_variables[$key] = $value;

		if (array_key_exists($key, self::$_not_set_variables)) {
			unset(self::$_not_set_variables[$key]);
		}

		R::store($variable);
	}

	static function get($key, $default = null) {
		if (array_key_exists($key, self::$_not_set_variables)) {
			return $default;
		}

		if (array_key_exists($key, self::$_variables)) {
			return self::$_variables[$key];
		}

		else if (R::count('variables')) {
			$variable = R::findOne('variables', '`key` = :key', array(':key' => $key));

			if ($variable !== null) {
				self::$_variables[$key] = $variable->value;
				return $variable->value;
			}

			self::$_not_set_variables[$key] = true;
		}

		return $default;
	}

	static function del($key) {
		if (!array_key_exists($key, self::$_not_set_variables)) {
			if (array_key_exists($key, self::$_variables)) {
				unset(self::$_variables[$key]);
			}

			if (R::count('variables')) {
				$variable = R::findOne('variables', '`key` = :key', array(':key' => $key));

				if ($variable !== null) {
					R::trash($variable);
				}
			}

			self::$_not_set_variables[$key] = true;
		}
	}
}
