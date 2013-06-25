<?php
/**
 * Hooks
 */
function app_urls(&$urls) {
	$urls['/'] = array(
		'callback' => '_app_page_home',
		'type' => PAGE_CALLBACK_NORMAL,
	);

	$urls['about'] = array(
		'callback' => '_app_page_about',
		'type' => PAGE_CALLBACK_NORMAL,
	);
}


/**
 * Page callbacks
 */
function _app_page_home() {
	return Template::render('page-home');
}

function _app_page_about() {
	return Template::render('page-about');
}
