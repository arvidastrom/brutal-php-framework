<?php
class Template {
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
