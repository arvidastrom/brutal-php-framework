<?php
Class Session {
	public static function set($key, $val) {
		global $session_prefix;

		$_SESSION[$session_prefix][$key] = $val;
	}

	public static function get($key, $default = NULL) {
		global $session_prefix;

		if (array_key_exists($session_prefix, $_SESSION) && array_key_exists($key, $_SESSION[$session_prefix])) {
			return $_SESSION[$session_prefix][$key];
		}

		return $default;
	}
	
	public static function del($key) {
		global $session_prefix;

		unset($_SESSION[$session_prefix][$key]);
	}
}
