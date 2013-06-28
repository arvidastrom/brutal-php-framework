<?php
class Template {
	/**
	 * Properties
	 */
	private static $body_classes = array();
	private static $html_variables = array();
	private static $page_variables = array();

	/**
	 * Public methods
	 */
	static function set_html_variable($vars, $value = null) {
		if ($value !== null) {
			$vars = array($vars => $value);
		}

		foreach ($vars as $key => $var) {
			self::$html_variables[$key] = $var;
		}
	}

	static function get_html_variables() {
		return self::$html_variables;
	}

	static function set_page_variable($vars, $value = null) {
		if ($value !== null) {
			$vars = array($vars => $value);
		}

		foreach ($vars as $key => $var) {
			self::$page_variables[$key] = $var;
		}
	}

	static function get_page_variables() {
		return self::$page_variables;
	}

	static function add_body_class($class) {
		self::$body_classes[] = $class;

		self::$body_classes = array_unique(self::$body_classes);
	}

	static function get_body_classes() {
		return self::$body_classes;
	}

	static function render($template, $variables = array()) {
		if (!file_exists(SITE_ROOT . '/app/themes/' . self::get_theme() . '/templates/' . $template . '.tpl.php')) {
			return 'Template file not found: app/themes/' . self::get_theme() . '/templates/' . $template . '.tpl.php';
		}
		
		extract($variables);

		ob_start();

		include(SITE_ROOT . '/app/themes/' . self::get_theme() . '/templates/' . $template . '.tpl.php');

		return ob_get_clean();
	}

	static function set_theme($theme_name) {
		global $theme;

		$theme = $theme_name;
	}

	static function get_theme() {
		global $theme;

		return $theme;
	}

	static function theme_path() {
		return 'app/themes/' . self::get_theme();
	}
}
