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
  drupal_set_message('Import open id connect account started.');
  watchdog('procoid', 'Import open id connect account by drush command started.');
  procoid_customcron();
  drupal_set_message('Import ended.');
  watchdog('procoid', 'Import ended.');
}
