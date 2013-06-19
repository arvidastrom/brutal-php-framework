<?php
class Core {
	/**
	 * Theme properties and methods
	 */
	private static $_body_classes = array();
	private static $_html_vars = array();
	private static $_page_vars = array();
	private static $_content = '';
	private static $_js = array();
	private static $_css = array();
	private static $_deliver_type = PAGE_CALLBACK_NORMAL;

	static function add_js($mixed, $js = null) {
		if (!is_array($mixed)) {
			self::$_js[$mixed] = $js;
		}
		else {
			foreach ($mixed as $key => $js) {
				self::$_js[$key] = $js;
			}
		}
	}

	static function add_css($mixed, $css = null) {
		if (!is_array($mixed)) {
			self::$_css[$mixed] = $css;
		}
		else {
			foreach ($mixed as $key => $css) {
				self::$_css[$key] = $css;
			}
		}
	}

	static function add_html_variables($vars) {
		foreach ($vars as $key => $var) {
			self::$_html_vars[$key] = $var;
		}
	}

	static function add_page_variables($vars) {
		foreach ($vars as $key => $var) {
			self::$_page_vars[$key] = $var;
		}
	}

	static function add_body_class($class) {
		self::$_body_classes[] = $class;

		self::$_body_classes = array_unique(self::$_body_classes);
	}

	static function set_title($title = null, $exclude_base_title = false) {
		if (empty($title)) {
			if ($exclude_base_title === true) {
				$title = '';
			}
			else {
				$title = SITE_TITLE;
			}
		}
		else {
			if ($exclude_base_title !== true) {
				$title .= ' | ' . SITE_TITLE;
			}
		}

		self::$_html_vars['page_title'] = $title;
	}

	static function set_content($content) {
		self::$_content = $content;
	}

	static function get_content() {
		return self::$_content;
	}


	/**
	 * Hooks properties and methods
	 */

	// available hooks
	private static $_hooks = array(
		'init' => array(),
		'urls' => array(),
		'html_variables' => array(),
		'page_variables' => array(),
		'js' => array(),
		'css' => array(),
	);

	private static function add_hooks() {
		// get all defined functions
		$functions = get_defined_functions();

		// add functions to hooks array
		if (!empty($functions['user'])) {
			foreach ($functions['user'] as $function) {
				$hook = substr($function, strpos($function, '_') + 1);

				// check if this is a hook
				if (array_key_exists($hook, self::$_hooks)) {
					self::$_hooks[$hook][] = $function;
				}
			}
		}
	}

	static function get_hook_functions($hook) {
		return self::$_hooks[$hook];
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
		foreach (self::get_hook_functions('urls') as $function) {
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
	 * App javascript. Creates a global js object with site information
	 */
	private static function add_app_js() {
		$js = "
		var App = {
			Url: {
				_baseurl: '". Url::generate() ."',
				_current: '". Url::current(true) ."',
				_path: ". json_encode(Url::path()) .",

				generate: function (path, query) {
					if (arguments.length == 0) {
						path = null;
					}

					var url = this._baseurl;

					if (path != null && path.substr(0, 1) != '/') {
						path = '/' + path;
					}

					if (path != null) {
						url += path;
					}

					if (arguments.length == 2) {
						var build_query = function (obj, num_prefix, temp_key) {
						  var output_string = [];
						 
						  Object.keys(obj).forEach(function (val) {
						 
						    var key = val;
						 
						    num_prefix && !isNaN(key) ? key = num_prefix + key : '';
						 
						    var key = encodeURIComponent(key.replace(/[!'()*]/g, escape));
						    temp_key ? key = temp_key + '[' + key + ']' : '';
						 
						    if (typeof obj[val] === 'object') {
						      var query = build_query(obj[val], null, key);
						      output_string.push(query);
						    }
						 	
						    else if (typeof obj[val] === 'number') {
						      var value = obj[val];
						      output_string.push(key + '=' + value);
						    }

						    else {
						      var value = encodeURIComponent(obj[val].replace(/[!'()*]/g, escape));
						      output_string.push(key + '=' + value);
						    }
						 
						  });
						 
						  return output_string.join('&');
						}

						url += '?' + build_query(query);
					}

					return url;
				},
				current: function (include_query_string) {
					if (include_query_string === true) {
						return this._current;
					}

					return this._current.split('?')[0];
				},
				path: function (index) {
					if (!isNaN(index) && isFinite(index)) {
						return this._path[index];
					}

					return this._path;
				},
				hash: function () {
					return document.location.hash.replace('#', '');
				}
			}
		};
		";

		self::add_js('core', $js);
	}

	private static function error_404() {
		self::set_title('Page not found');
		self::$_content = 'Page not found: ' . Url::request_path();
	}


	/**
	 * Init method, called after configuration and files are loaded
	 */
	static function init() {
		// add all available hooks on the system
		self::add_hooks();

		// set the default title
		self::set_title();

		// add the app script to the html variables
		self::add_app_js();

		// call the init hook
		foreach (self::get_hook_functions('init') as $function) {
			call_user_func_array($function, array());
		}

		// see if we got a matching url route
		$page_info = self::find_route();

		if ($page_info !== false) {
			$content = call_user_func_array($page_info['callback'], array());

			if ($content === false) {
				self::error_404();
			}
			else {
				if (array_key_exists('type', $page_info)) {
					self::$_deliver_type = $page_info['type'];
				}

				if (array_key_exists('title', $page_info)) {
					self::set_title($page_info['title']);
				}

				self::$_content = $content;
			}
		}
		else {
			self::error_404();
		}
	}

	// print the final page
	static function view() {
		if (self::$_deliver_type == PAGE_CALLBACK_NORMAL) {
			$page_vars = self::$_page_vars;
			$page_vars['content'] = self::$_content;

			// let modules interact before the final output
			foreach (self::get_hook_functions('page_variables') as $function) {
				call_user_func_array($function, array(&$page_vars));
			}

			$page = Template::render('page', $page_vars);

			$html_vars = self::$_html_vars;
			$html_vars['content'] = $page;
			$html_vars['classes'] = '';

			foreach (self::$_body_classes as $class) {
				$html_vars['classes'] .= ' ' . $class;
			}

			$html_vars['classes'] = trim($html_vars['classes']);

			$js = self::$_js;

			// let modules interact with the js before the final output
			foreach (self::get_hook_functions('js') as $function) {
				call_user_func_array($function, array(&$js));
			}

			foreach ($js as &$script) {
				if (filter_var($script, FILTER_VALIDATE_URL) === false && substr($script, 0, 2) != '//') {
					$script = "<script>\r\n" . $script . "\r\n</script>";
				}
				else {
					$script = '<script src="' . $script . '"></script>';
				}
			}

			$html_vars['js'] = "\r\n" . implode("\r\n", $js) . "\r\n";

			$css = self::$_css;

			// let modules interact with the js before the final output
			foreach (self::get_hook_functions('css') as $function) {
				call_user_func_array($function, array(&$css));
			}

			foreach ($css as &$style) {
				if (filter_var($style, FILTER_VALIDATE_URL) === false && substr($style, 0, 2) != '//') {
					$style = '<style>' . $style . '</style>';
				}
				else {
					$style = '<link rel="stylesheet" href="' . $style . '">';
				}
			}

			$html_vars['css'] = "\r\n" . implode("\r\n", $css) . "\r\n";

			// let modules interact before the final output
			foreach (self::get_hook_functions('page_variables') as $function) {
				call_user_func_array($function, array(&$html_vars));
			}

			$html = Template::render('html', $html_vars);

			header('Content-Type: text/html; charset=UTF-8');

			print $html;
		}
		elseif (self::$_deliver_type == PAGE_CALLBACK_BLANK) {
			print self::$_content;
		}
		elseif (self::$_deliver_type == PAGE_CALLBACK_AJAX) {

			header('Content-type: application/json');
			print json_encode(self::$_content);
		}
	}
}

