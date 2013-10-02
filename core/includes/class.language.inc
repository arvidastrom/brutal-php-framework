<?php
 /**
  * Language class.
  *
  * Allows for translations by using .po files.
  * 
  * @package Core
  */
class Language {
	/**
	 * Stores all translation for all languages.
	 *
	 * @access private
	 *
	 * @var array
	 */
	private static $translations = array();

	/**
	 * Holds the current locale that is being used for translations
	 *
	 * @access private
	 *
	 * @var mixed
	 */
	private static $locale = null;

	/**
	 * Sets the current locale that will be used when translate() is being called
	 *
	 * @param string $locale File prefix of the language file in the translations directory (en_US)
	 */
	static function setLocale($locale) {
		self::$locale = $locale;
	}

	/**
	 * Returns the current locale
	 *
	 * @return string Current locale
	 */
	static function getLocale() {
		return self::$locale;
	}

	/**
	 * Translate a message id to the currently set language
	 *
	 * * Reads the translation file the first time translate is being called to avoid unnecessary
	 * * file reads if translation is not needed on the current page request.
	 *
	 * @todo Add support for plural.
	 *
	 * @param string $msgid The msgid in the .po file
	 * @param string $locale Optionally specify another language than the default one.
	 *
	 * @return string The translated message if it exists in the current locale, if not, returns the msgid.
	 */
	static function translate($msgid, $locale = null) {
		if ($locale === null) {
			$locale = self::$locale;

			if ($locale === null) {
				global $locale;

				if (!empty($locale)) {
					self::$locale = $locale;
				}
				else {
					return $msgid;
				}
			}
		}

		if (!array_key_exists($locale, self::$translations)) {
			self::readTranslations($locale);
		}

		if (array_key_exists($msgid, self::$translations[$locale])) {
			return self::$translations[$locale][$msgid]['msgstr'][0];
		}

		return $msgid;
	}

	/**
	 * Reads translations from .po files and stores them in the $translations property.
	 *
	 * @access private
	 *
	 * @param string $locale The locale of the .po file
	 */
	private static function readTranslations($locale) {
		if (!self::validateLocale($locale)) {
			throw new Exception('Invalid locale: ' . $locale);
		}

		$file = SITE_ROOT . '/app/translations/' . $locale . '.po';

		if (file_exists($file)) {
			$poparser = new PoParser();
			self::$translations[$locale] = $poparser->read($file);
		}
		else {
			self::$translations[$locale] = array();
			throw new Exception('Missing language file: ' . $file);
		}
	}

	/**
	 * Validates the supplied locale
	 *
	 * @access private
	 *
	 * @param string $locale
	 *
	 * @return bool
	 */
	private static function validateLocale($locale) {
		$locale = $explode('_', $locale);

		// Make sure we got 2 indexes
		if (sizeof($locale) !== 2) {
			return false;
		}

		// Check that the size of both is 2
		if (strlen($locale[0]) !== 2 || strlen($locale[1]) !== 2) {
			return false;
		}

		if (!ctype_lower($locale[0]) || !ctype_upper($locale[1])) {
			return false;
		}

		return true;
	}
}
