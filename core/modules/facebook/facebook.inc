<?php
require SITE_ROOT . '/core/modules/facebook/facebook.php';

function facebook_init() {
	$js = "
		window.fbAsyncInit = function() {
			// init the FB JS SDK
			FB.init({
				appId      : '" . FACEBOOK_APP_ID . "', // App ID from the app dashboard
				channelUrl : '" . Url::generate('modules/facebook/channel.html') . "', // Channel file for x-domain comms
				status     : true, // Check Facebook Login status
				xfbml      : true  // Look for social plugins on the page
			});
	
			if (jQuery) {
				(function () {
					jQuery.fn.fbready = function(callback) {

				};
				// Additional initialization code such as adding Event Listeners goes here
				jQuery(window).trigger('fbready');
			}
		};

		// Load the SDK asynchronously
		(function(d, s, id){
			var js, fjs = d.getElementsByTagName(s)[0];
			if (d.getElementById(id)) {return;}
			js = d.createElement(s); js.id = id;
			js.src = '//connect.facebook.net/" . FACEBOOK_LOCALE . "/all.js';
			fjs.parentNode.insertBefore(js, fjs);
		}(document, 'script', 'facebook-jssdk'));
	";

	Core::add_js('facebook', $js);
}

function facebook_js(&$js) {
	$js['facebook'] = "
		(function () {
			var fbready = false;
			var readycallbacks = [];

			App.fbready = function (callback) {
				if (fbready) {
					callback();
				}
				else {
					readycallbacks.push(callback);
				}
			};

			window.fbAsyncInit = function() {
				// init the FB JS SDK
				FB.init({
					appId      : '" . FACEBOOK_APP_ID . "', // App ID from the app dashboard
					channelUrl : '" . Url::generate('modules/facebook/channel.html') . "', // Channel file for x-domain comms
					status     : true, // Check Facebook Login status
					xfbml      : true  // Look for social plugins on the page
				});
		
				fbready = true;
				
				if (readycallbacks.length) {
					for (var i=0, c=readycallbacks.length; i<c; i++) {
						readycallbacks[i]();
					}
				}
			};
		})();

		// Load the SDK asynchronously
		(function(d, s, id){
			var js, fjs = d.getElementsByTagName(s)[0];
			if (d.getElementById(id)) {return;}
			js = d.createElement(s); js.id = id;
			js.src = '//connect.facebook.net/" . FACEBOOK_LOCALE . "/all.js';
			fjs.parentNode.insertBefore(js, fjs);
		}(document, 'script', 'facebook-jssdk'));
	";
}

function facebook_urls(&$urls) {
	$urls['modules/facebook/channel.html'] = array(
		'callback' => '_facebook_page_channel',
		'type' => PAGE_CALLBACK_BLANK,
	); 
}

function _facebook_page_channel() {
	$cache_expire = 60*60*24*365;
	header("Pragma: public");
	header("Cache-Control: max-age=".$cache_expire);
	header('Expires: ' . gmdate('D, d M Y H:i:s', time()+$cache_expire) . ' GMT');

	return '<script src="//connect.facebook.net/' . FACEBOOK_LOCALE . '/all.js"></script>';
}

function facebook_get_object() {
	static $facebook = null;

	if ($facebook === null) {
		$facebook = new Facebook(array(
			'appId'  => FACEBOOK_APP_ID,
			'secret' => FACEBOOK_APP_SECRET,
		));
	}

	return $facebook;
}

function facebook_likes_page() {
	$facebook = facebook_get_object();

	$signed_request = $facebook->getSignedRequest();

	if ($signed_request['page']['liked'] == 1) {
		return true;
 	}
	else if (isset($signed_request['page']['liked'])) {
		return false;
	}

	return true;
}

