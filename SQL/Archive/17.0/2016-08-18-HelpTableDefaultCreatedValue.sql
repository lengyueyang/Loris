SET SESSION sql_mode = 'ALLOW_INVALID_DATES';
ALTER TABLE help CHANGE `created` `created` DATETIME DEFAULT NULL;
ALTER TABLE help CHANGE `updated` `updated` DATETIME DEFAULT NULL;
UPDATE help SET created = NULL WHERE created = '0000-00-00 00:00:00';
