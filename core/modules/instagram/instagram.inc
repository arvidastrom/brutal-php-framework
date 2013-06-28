<?php
require_once(SITE_ROOT . '/core/modules/instagram/instagram.class.php');

function instagram_get_object() {
	static $instagram = null;

	if ($instagram === null) {
		$settings = instagram_settings();

		if ($settings != null) {
			$instagram = new Instagram(array(
				'apiKey' => $settings['client_id'],
				'apiSecret' => $settings['client_secret'],
				'apiCallback' => $settings['redirect_uri'],
			));

			$instagram->user = false;

			if (isset($_GET['code'])) {
				$access_token = $instagram->getOAuthToken($_GET['code']);
				$instagram->setAccessToken($access_token);

				setcookie('instagram-api-access-token', $access_token->access_token, 0, '/');

				Url::redirect(null, false);
			}
			else if (!empty($_COOKIE['instagram-api-access-token'])) {
				$access_token = $_COOKIE['instagram-api-access-token'];
				$instagram->setAccessToken($access_token);

				try {
					$user = $instagram->getUser();
				} catch (Exception $e) {}

				if (!empty($user->data)) {
					$instagram->user = $user->data;
				}
				else {
					setcookie('instagram-api-access-token', '', time() - 3600, '/');
				}
			}
		}
	}

	return $instagram;
}

function instagram_settings($arr = null) {
	static $settings = null;

	if ($arr !== null) {
		$settings = $arr;
	}
	else {
		return $settings;
	}
}
