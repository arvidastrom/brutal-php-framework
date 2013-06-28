var App = {
	Url: {
		_baseurl: '{#urlbase#}',
		_current: '{#urlcurrent#}',
		_path: '{#urlpath#}',

		generate: function (path, options) {
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

			if (typeof options === 'object' && options.query) {
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
	},
	Settings: {#settings#}
};
