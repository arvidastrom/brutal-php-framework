<?php
class Template {
	static function render($template, $variables = array()) {
		if (!file_exists(TEMPLATE_DIR . '/' . $template . '.tpl.php')) {
			return '';
		}
		
		extract($variables);

		ob_start();

		include(TEMPLATE_DIR . '/' . $template . '.tpl.php');

		return ob_get_clean();
	}
}