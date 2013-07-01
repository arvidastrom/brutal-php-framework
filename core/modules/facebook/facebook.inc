<?php
// include facebook php sdk
require SITE_ROOT . '/core/modules/facebook/facebook_sdk/facebook.php';

/**
 * Hooks
 */
function facebook_hook_js(&$js) {
	$settings = facebook_settings();

	if ($settings != null) {
		// setup array for replacing variables inside the js file
		$variables = array(
			'{#appid#}' => $settings['app_id'],
			'{#channel#}' => Url::generate('modules/facebook/channel.html'),
		);

		// get the file into a string
		$facebook_js = file_get_contents(SITE_ROOT . '/core/modules/facebook/facebook.js');

		// replace all variables
		$facebook_js = str_replace(array_keys($variables), array_values($variables), $facebook_js);

		// add some js!
		$js['script'][] = array(
			'type' => 'inline',
			'data' => $facebook_js,
		);
	}
}

function facebook_hook_urls(&$urls) {
	$settings = facebook_settings();

	if ($settings != null) {
		$urls['modules/facebook/channel.html'] = array(
			'callback' => 'facebook_page_channel',
			'type' => Brutal::PAGE_CALLBACK_BLANK,
		);
	}
}


/**
 * Page callbacks
 */
function facebook_page_channel() {
	$settings = facebook_settings();

	if ($settings != null) {
		$cache_expire = 60*60*24*365;
		header("Pragma: public");
		header("Cache-Control: max-age=".$cache_expire);
		header('Expires: ' . gmdate('D, d M Y H:i:s', time()+$cache_expire) . ' GMT');

		return '<script src="//connect.facebook.net/' . $settings['app_locale'] . '/all.js"></script>';
	}
}


/**
 * Functions
 */
function facebook_settings($arr = null) {
	static $settings = null;

	if ($arr !== null) {
		$settings = $arr;
	}
	else {
		return $settings;
	}
}

function facebook_get_object() {
	static $facebook = null;

	if ($facebook === null) {
		$settings = facebook_settings();

		if ($settings != null) {
			$facebook = new Facebook(array(
				'appId'  => $settings['app_id'],
				'secret' => $settings['app_secret'],
			));
		}
	}

	return $facebook;
}

function facebook_likes_page() {
	$facebook = facebook_get_object();

	if ($facebook != null) {
		$signed_request = $facebook->getSignedRequest();

		if ($signed_request['page']['liked'] == 1) {
			return true;
	 	}
		else if (isset($signed_request['page']['liked'])) {
			return false;
		}
	}

	return true;
}

