<?php
class Url {
	private static $_url = null;

	/**
	 * Generates an site url
	 *
	 * Generates an url to the site root if the path parameter is omitted
	 * if not, generates an absolute url to the specified path.
	 * Checks if the SITE_URL is defined and uses that as the base url instead.
	 *
	 * @param string $path The path to get a url for, relative to the site root.
	 *
	 * @return string The url to the site root or the the chosen path.
	 */
	static function generate($path = null, $query = null) {
		$url = self::$_url;

		if ($url == null) {
			// use the SITE_URL constant instead of a calculate site root if it is defined
			if (defined('SITE_URL')) {
				$url = SITE_URL;

				if (self::https()) {
					$url = str_replace('http://', 'https://', $url);
				}
			}
			else {
				$url = (self::https()) ? 'https' : 'http';

				$url .= '://' . $_SERVER['HTTP_HOST'];

				if ($_SERVER['SERVER_PORT'] != '80') {
					$url .= ':' . $_SERVER['SERVER_PORT'];
				}

				// as the executed file url is always the index.php
				// file as defined in the rewrite rule we can safely
				// remove the filename from the url
				$url .= str_replace('/index.php', '', $_SERVER['PHP_SELF']);
			}

			self::$_url = $url;
		}

		$site_url = $url;

		if ($path !== null && substr($path, 0, 1) != '/') {
			$path = '/' . $path;
		}

		if ($path !== null) {
			$site_url .= $path;
		}

		$site_url = urldecode($site_url);

		if ($query !== null) {
			$site_url .= '?' . http_build_query($query);
		}

		return $site_url;
	}

	/**
	 * Calculates the current url
	 *
	 * Calculates the current url that is being requested. If the
	 * included_query_string parameter is true it includes any query string
	 * thats also in the url.
	 *
	 * @param string $include_query_string Flag to include the query string in the returned url.
	 *
	 * @return string The url that is currently being requested.
	 */
	static function current($include_query_string = false) {
		// as the executed file url is always the index.php
		// file as defined in the rewrite rule we can safely
		// remove the filename from the url
		$path = str_replace('/index.php', '', $_SERVER['PHP_SELF']);

		$path = str_replace($path, '', $_SERVER['REQUEST_URI']);

		if (!$include_query_string) {
			$path = strtok($path, '?');
		}

		return self::generate($path);
	}

	/**
	 * Calculates the requested path relative to the site root
	 *
	 * @return string The path that is currently being requested.
	 */
	static function request_path() {
		$path = str_replace('/index.php', '', $_SERVER['PHP_SELF']);
		$path = str_replace($path, '', $_SERVER['REQUEST_URI']);
		$path = strtok($path, '?');
		$path = urldecode($path);
		$path = trim($path, '/');

		if (empty($path)) {
			$path = '/';
		}

		return $path;
	}

	static function https() {
		return (!empty($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) == 'on');
	}


	/**
	 * Calculates the requested path relative to the site root
	 *
	 * @param int $index The fragment of the current requested path to retrieve
	 *
	 * @return mixed An array if an index is not supplied, if supplied a fragment is returned. null is returned if the index is not defined.
	 */
	static function path($index = null) {
		$path = self::request_path();
		$path = trim($path, '/');

		if (empty($path)) {
			$path = array();
		}
		else {
			$path = explode('/', $path);
		}
		
		if ($index !== null) {
			if (array_key_exists($index, $path)) {
				return $path[$index];
			}

			return null;
		}

		return $path;
	}

	/**
	 * Reloads the page or redirects the browser
	 *
	 * @param string $url The url to redirect the browser to, if omitted, reloads the current page
	 *
	 * @return void
	 */
	static function redirect($url = null) {
		// reload the current page
		if ($url === null) {
			$url = self::current(true);
		}
		elseif (stripos('http', $url) === -1) {
			$url = self::generate($url);
		}
		
		header('Location: ' . $url);
		exit;
	}
}
