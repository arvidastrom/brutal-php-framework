<?php
Class Session {
	public static function set($key, $val) {
		$_SESSION[SESSION_PREFIX][$key] = $val;
	}

	public static function get($key, $default = NULL) {
		if (array_key_exists($key, $_SESSION[SESSION_PREFIX])) {
			return $_SESSION[SESSION_PREFIX][$key];
		}

		return $default;
	}
	
	public static function del($key) {
		unset($_SESSION[SESSION_PREFIX][$key]);
	}
}
