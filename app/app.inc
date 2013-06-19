<?php
/**
 * Hooks
 */
function app_urls(&$urls) {
	$urls['/'] = array(
		'callback' => '_app_page_home',
		'type' => PAGE_CALLBACK_NORMAL,
	);
}

function app_init() {
	$js = array(
		'jquery' => '//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js',
		'main' => Url::generate('app/js/main.js'),
	);
	Core::add_js($js);

	$css = array(
		'reset' => Url::generate('app/css/reset.css'),
		'main' => Url::generate('app/css/main.css'),
	);
	Core::add_css($css);
}


/**
 * Page callbacks (Normal functions)
 */

function _app_page_home() {
	return Template::render('page-home');
}
