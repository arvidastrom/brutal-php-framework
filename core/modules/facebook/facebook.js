(function () {
	var fbready = false;
	var readycallbacks = [];

	App.Facebook = {
		ready: function (callback) {
			if (fbready) {
				callback.apply(this);
			}
			else {
				readycallbacks.push(callback);
			}
		},
		statusOrLogin: function (callback) {
			FB.getLoginStatus(function(response) {
				if (response.status === 'connected') {
					callback.apply(FB, [response]);
				}
				else {
					FB.login(function(response) {
						callback.apply(FB, [response]);
					});
				}
			});
		},
		inCanvas: function () {
			return (window.location != window.parent.location);
		}
	};

	window.fbAsyncInit = function() {
		// init the FB JS SDK
		FB.init({
			appId      : '{#appid#}', // App ID from the app dashboard
			channelUrl : '{#channel#}', // Channel file for x-domain comms
			status     : true, // Check Facebook Login status
			xfbml      : true  // Look for social plugins on the page
		});

		fbready = true;
		
		if (readycallbacks.length) {
			for (var i=0, c=readycallbacks.length; i<c; i++) {
				readycallbacks[i].apply(App.Facebook);
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
