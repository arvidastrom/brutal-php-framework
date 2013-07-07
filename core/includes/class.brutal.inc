<?php
 /**
  * Brutal class.
  *
  * Handles all core functionality, such as url rewrites and hook system.
  * 
  * @package Core
  */
class Brutal {
	/**
	 * Normal page view
	 *
	 * Prints html and page template files wrapped around the content
	 */
	const PAGE_CALLBACK_NORMAL = 0;

	/**
	 * Json page view
	 *
	 * Prints the returned data as json encoded without html and page template
	 */
	const PAGE_CALLBACK_JSON = 1;

	/**
	 * Blank page view
	 *
	 * Prints the returned data as plain text encoded without html and page template
	 */
	const PAGE_CALLBACK_BLANK = 2;

	/**
	 * Stores all javascript
	 *
	 * @access private
	 * @var array
	 */
	private static $js = array(
		'script' => array(),
		'settings' => array(),
	);

	/**
	 * Stores all css
	 *
	 * @access private
	 * @var array
	 */
	private static $css = array();

	/**
	 * The page delivery type
	 *
	 * @access private
	 * @var string
	 */
	private static $page_delivery_type;

	/**
	 * Add javascript files / variables to the site.
	 *
	 * Add internal/external, inline javascript files or settings passed on to the
	 * App.Settings javascript object.
	 *
	 * * Settings are merged into one final, so beware of overwriting previous settings.
	 *
	 * Examples:
	 *
	 * * Add an internal script
	 *
	 * <code>
	 * $js = array(
	 * 	'type' => 'internal',
	 * 	'data' => 'app/js/phpjs.js',
	 * );
	 * Brutal::add_js($js);
	 * </code>
	 *
	 * * Add an external script
	 *
	 * <code>
	 * $js = array(
	 * 	'type' => 'external',
	 * 	'data' => '//ajax.googleapis.com/ajax/libs/jquery/latest/jquery.min.js',
	 * );
	 * Brutal::add_js($js);
	 * </code>
	 *
	 * * Add inline script
	 *
	 * <code>
	 * $js = array(
	 * 	'type' => 'inline',
	 * 	'data' => 'document.write("Hello World");',
	 * );
	 * Brutal::add_js($js);
	 * </code>
	 *
	 * * Add settings
	 *
	 * <code>
	 * $js = array(
	 * 	'page-urls' => array(
	 *		'page/info',
	 *		'page/contact',
	 * 	),
	 * );
	 * Brutal::add_js($js, 'settings');
	 * </code>
	 *
	 * @param array $info  Array containing information about the script added.
	 * @param string $type Type of js to add.
	 */
	static function add_js($info, $type = 'script') {
		// handle setting explicitly
		if ($type == 'settings') {
			$current_settings = self::$js['settings'];

			// merge the new settings with the current ones
			self::$js['settings'] = array_merge_recursive($current_settings, $info);
		}
		else {
			// create a default options array
			$defaults = array(
				'type' => 'internal',
			);

			$options = array_merge($defaults, $info);

			// add it to the js array
			self::$js['script'][] = $options;
		}
	}

	/**
	 * Get all or specific js previously added.
	 *
	 * Returns all js or script/settings.
	 *
	 * @param string|null $type The type to retrieve. Either 'script' or 'settings'.
	 *
	 * @return array The specified type or all javascript.
	 */
	static function get_js($type = null) {
		if ($type !== null) {
			return self::$js[$type];
		}
		
		return self::$js;
	}

	/**
	 * Add css files/styles.
	 *
	 * Add internal/external or inline styles to be outputted in the template.
	 *
	 * * The media key in {@$info} specifies the media query to use.
	 *
	 * Examples:
	 *
	 * * Add an internal style
	 *
	 * <code>
	 * $style = array(
	 * 	'type' => 'internal',
	 *	'media' => 'all',
	 * 	'data' => 'app/css/reset.css',
	 * );
	 * Brutal::add_css($style);
	 * </code>
	 *
	 * * Add an external style
	 *
	 * <code>
	 * $style = array(
	 * 	'type' => 'external',
	 *	'media' => 'screen',
	 * 	'data' => 'http://styles.cdn.com/style.css',
	 * );
	 * Brutal::add_css($style);
	 * </code>
	 *
	 * * Add inline style
	 *
	 * <code>
	 * $style = array(
	 * 	'type' => 'inline',
	 * 	'data' => 'body { color: #fff; };',
	 * );
	 * Brutal::add_css($style);
	 * </code>
	 *
	 * @param array $info  Array containing information about the style added.
	 */
	static function add_css($info) {
		// create a default options array
		$defaults = array(
			'media' => 'all',
			'type' => 'internal',
		);

		// overwrite any defaults
		$options = array_merge($defaults, $info);

		// add it to the css array
		self::$css[] = $options;
	}

	/**
	 * Get all styles previously added.
	 *
	 * Returns all added styles.
	 *
	 * @return array All css added using the Brutal::add_css method.
	 */
	static function get_css() {
		return self::$css;
	}

	/**
	 * Set the site title, if no arguments are passed it will use the default
	 */
	static function set_title($title = null, $exclude_base = false) {
		global $site_title;

		if (empty($title)) {
			if ($exclude_base === true) {
				$title = '';
			}
			else {
				$title = $site_title;
			}
		}
		else {
			if ($exclude_base !== true) {
				$title .= ' | ' . $site_title;
			}
		}

		Template::set_html_variable('page_title', $title);
	}


	/**
	 * Modules section
	 */

	// available hooks
	private static $hooks = array(
		'hook_init' => array(),
		'hook_urls' => array(),
		'hook_html_variables' => array(),
		'hook_page_variables' => array(),
		'hook_js' => array(),
		'hook_css' => array(),
	);

	private static function include_modules() {
		global $modules;

		foreach ($modules as $module) {
			require_once(SITE_ROOT . '/core/modules/' . $module . '/' . $module . '.inc');
		}
	}

	private static function find_hooks() {
		global $modules;

		// don't mess with the global modules
		$enabled_modules = $modules;

		// add in our app and theme last as them act as modules
		$enabled_modules[] = 'app';
		$enabled_modules[] = 'theme';

		// loop through all hooks and check if each module implements it
		foreach (self::$hooks as $hook => $functions) {
			foreach ($enabled_modules as $module) {
				$function = $module . '_' . $hook;

				if (function_exists($function)) {
					self::$hooks[$hook][] = $function;
				}
			}
		}
	}

	static function get_hook_functions($hook) {
		return self::$hooks[$hook];
	}

	static function add_hooks($hooks) {
		$new_hooks = array();

		foreach ($hooks as $hook) {
			$new_hooks[$hook] = array();
		}

		self::$hooks = array_merge(self::$hooks, $new_hooks);
	}


	/**
	 * Url routing
	 */
	private static function find_route() {
		// the current request path
		$request_path = Url::request_path();

		// all registered urls in the system
		$urls = array();

		// run all hooks to get all defined url's
		foreach (self::get_hook_functions('hook_urls') as $function) {
			call_user_func_array($function, array(&$urls));
		}

		// loop through and see if we have a match
		foreach ($urls as $path => $info) {
			// an exact match always preceds a wildcard match
			if ($path == $request_path) {
				$page_info = $info;
				break;
			}
			else {
				// perform a manual match
				$match = array();
				$replace = array();

				$args = explode('/', $path);

				foreach ($args as $index => &$arg) {
					if ($arg == '*' && Url::path($index)) {
						$arg = '%' . $index;

						$match[] = $arg;
						$replace[] = Url::path($index);
					}
				}

				$path = implode('/', $args);
				$path = str_replace($match, $replace, $path);

				if ($path == $request_path) {
					$page_info = $info;
				}
			}
		}

		// did we get a match?
		if (isset($page_info)) {
			return $page_info;
		}

		return false;
	}


	/**
	 * Adds a global js object with js settings and url functions
	 */
	private static function get_app_js() {
		// setup array for replacing variables inside the js file
		$variables = array(
			'{#urlbase#}' => rtrim(Url::generate('/', array('absolute' => true)), '/'),
			'{#urlcurrent#}' => Url::current(true),
			'{#urlpath#}' => json_encode(Url::path()),
			'{#settings#}' => json_encode(self::get_js('settings')),
		);

		// get the file into a string
		$js = file_get_contents(SITE_ROOT . '/core/js/app.js');

		// replace all variables
		$js = str_replace(array_keys($variables), array_values($variables), $js);

		// add some js!
		return $js;
	}

	private static function error_404() {
		self::set_title('Page not found');
		return 'Page not found: ' . Url::request_path();
	}

	/**
	 * Parse theme info file and add css and javascript
	 */
	private static function parse_theme_config() {
		$theme = Template::get_theme();

		// parse the selected theme's info file and add css and javascripts
		$config_file = SITE_ROOT . '/app/themes/' . $theme . '/theme.config.inc';

		include $config_file;

		foreach ($css as $stylesheet) {
			// if the stylesheet is an array, we have media queries
			if (is_array($stylesheet)) {
				foreach ($stylesheet as $media => $media_stylesheet) {
					$css_info = self::parse_theme_css($media, $media_stylesheet);
				}
			}
			// if its not, no media query was defined. Default to all.
			else {
				$css_info = self::parse_theme_css('all', $stylesheet);
			}

			self::add_css($css_info);
		}

		foreach ($js as $script) {
			// check if its a internal or external js file
			if (filter_var($script, FILTER_VALIDATE_URL) === false && substr($script, 0, 2) != '//') {
				$js_info = array(
					'type' => 'internal',
					'data' => Template::theme_path() . '/' . $script,
				);
			}
			else {
				$js_info = array(
					'type' => 'external',
					'data' => $script,
				);
			}

			self::add_js($js_info);
		}
	}

	private static function parse_theme_css($media, $stylesheet) {
		// check if its a internal or external css file
		if (filter_var($stylesheet, FILTER_VALIDATE_URL) === false && substr($stylesheet, 0, 2) != '//') {
			$css_info = array(
				'media' => $media,
				'type' => 'internal',
				'data' => Template::theme_path() . '/' . $stylesheet,
			);
		}
		else {
			$css_info = array(
				'media' => $media,
				'type' => 'external',
				'data' => $stylesheet,
			);
		}

		return $css_info;
	}

	/**
	 * Init method, called after configuration and files are loaded
	 */
	static function init($mode = null) {
		// include modules
		self::include_modules();

		// add all available hooks on the system
		self::find_hooks();

		// set the default title
		self::set_title();

		// call the init hook
		foreach (self::get_hook_functions('hook_init') as $function) {
			call_user_func_array($function, array());
		}

		if ($mode == 'view') {
			self::view();
		}
	}

	// print the final page
	static function view() {
		// see if we got a matching url route
		$page_info = self::find_route();

		// set default delivery type
		$delivery_type = self::PAGE_CALLBACK_NORMAL;

		if ($page_info !== false) {
			// call the page callback
			$content = false;

			$current_theme = Template::get_theme();

			// check if we explicitly set a theme for this page
			if (array_key_exists('theme', $page_info)) {
				Template::set_theme($page_info['theme']);
			}

			if (function_exists($page_info['callback'])) {
				$content = call_user_func_array($page_info['callback'], array());
			}

			// check if we got any content back
			if ($content === false) {
				$content = self::error_404();

				Template::set_theme($current_theme);
			}
			else {
				// check if we explicitly added a page callback type
				if (array_key_exists('type', $page_info)) {
					$delivery_type = $page_info['type'];
				}
			}
		}
		else {
			$content = self::error_404();
		}

		// parse the theme config file
		self::parse_theme_config();

		if ($delivery_type == self::PAGE_CALLBACK_NORMAL) {

			/**
			 * Javascript
			 */
			// get all site javascript
			$js = self::$js;

			// let modules interact with the js before the final output
			foreach (self::get_hook_functions('hook_js') as $function) {
				call_user_func_array($function, array(&$js));
			}

			$app_js = array(
				'type' => 'inline',
				'data' => self::get_app_js(),
			);

			// add in the app js first
			array_unshift($js['script'], $app_js);

			$script_tags = array();

			foreach ($js['script'] as $script) {
				if ($script['type'] == 'inline') {
					$script_tag = "<script>\r\n" . $script['data'] . "\r\n</script>";
				}
				else {
					// generate absolute url for internal files
					if ($script['type'] == 'internal') {
						$script['data'] = Url::generate($script['data']);
					}

					$script_tag = '<script src="' . $script['data'] . '"></script>';
				}

				$script_tags[] = $script_tag;
			}

			$js_html = "\r\n" . implode("\r\n", $script_tags) . "\r\n";


			/**
			 * Css
			 */
			$css = self::$css;

			// let modules interact with the js before the final output
			foreach (self::get_hook_functions('hook_css') as $function) {
				call_user_func_array($function, array(&$css));
			}

			$style_tags = array();

			foreach ($css as $style) {
				if ($style['type'] == 'inline') {
					$style_tag = '<style media="' . $style['media'] . '">' . $style['data'] . '</style>';
				}
				else {
					if ($style['type'] == 'internal') {
						$style['data'] = Url::generate($style['data']);
					}

					$style_tag = '<link rel="stylesheet" media="' . $style['media'] . '" href="' . $style['data'] . '">';
				}

				$style_tags[] = $style_tag;
			}

			$css_html = "\r\n" . implode("\r\n", $style_tags) . "\r\n";


			/**
			 * Page template
			 */
			// get page variables from the template class
			$page_variables = Template::get_page_variables();

			// add the content to the page variable
			$page_variables['content'] = $content;

			// let modules interact before the final output
			foreach (self::get_hook_functions('hook_page_variables') as $function) {
				call_user_func_array($function, array(&$page_variables));
			}

			// render the page template
			$page_content = Template::render('page', $page_variables);


			/**
			 * Html template
			 */
			// get html variables from the template class
			$html_variables = Template::get_html_variables();

			// add the rendered page template to the html template
			$html_variables['content'] = $page_content;

			// no classes by default
			$html_variables['classes'] = '';

			foreach (Template::get_body_classes() as $class) {
				$html_variables['classes'] .= ' ' . $class;
			}

			// remove trailing and ending whitespace
			$html_variables['classes'] = trim($html_variables['classes']);

			// add js and css
			$html_variables['js'] = $js_html;
			$html_variables['css'] = $css_html;

			// let modules interact before the final output
			foreach (self::get_hook_functions('hook_page_variables') as $function) {
				call_user_func_array($function, array(&$html_variables));
			}

			$html = Template::render('html', $html_variables);

			header('Content-Type: text/html; charset=UTF-8');

			print $html;
		}
		elseif ($delivery_type == self::PAGE_CALLBACK_BLANK) {
			print $content;
		}
		elseif ($delivery_type == self::PAGE_CALLBACK_JSON) {
			header('Content-type: application/json');
			print json_encode($content);
		}
	}
}

