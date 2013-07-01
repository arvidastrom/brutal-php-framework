<?php
/**
 * Hooks
 */
function app_hook_init() {
	// Facebook settings
	facebook_settings(array(
		'app_id' => '',
		'app_secret' => '',
		'app_locale' => 'sv_SE',
	));

	// Instagram settings
	instagram_settings(array(
		'client_id' => '',
		'client_secret' => '',
		'redirect_uri' => Url::generate(),
	));
}

function app_hook_urls(&$urls) {
	$urls['/'] = array(
		'callback' => 'app_page_home',
		'type' => Brutal::PAGE_CALLBACK_NORMAL,
	);

	$urls['about'] = array(
		'callback' => 'app_page_about',
		'type' => Brutal::PAGE_CALLBACK_NORMAL,
	);
}


/**
 * Page callbacks
 */
function app_page_home() {
	return Template::render('page-home');
}

function app_page_about() {
	return Template::render('page-about');
}
