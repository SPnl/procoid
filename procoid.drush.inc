<?php

// Implements hook_drush_command
function procoid_drush_command() {
  $items = array();

  $items['procurios-import-users'] = array(
    'description' => "Import / update user accounts with current data in Procurios.",
    'aliases'     => array('procimportusers'),
  );

  return $items;
}

/*
 * Syncs not yet synced webform submissions.
 */
function drush_procoid_procurios_import_users() {
  watchdog('procoid', 'Import open id connect account by drush command started.');
  procoid_customcron();
  watchdog('procoid', 'Import ended.');
}
