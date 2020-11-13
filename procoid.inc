<?php

/**
 * Override for openid_connect_login_form_submit.
 *
 * Form submit handler: Log in with an OpenID Connect login provider.
 */
function procoid_openid_connect_login_form_submit(&$form, &$form_state) {
  openid_connect_save_destination();

  $client_name = $form_state['triggering_element']['#name'];
  $client = openid_connect_get_client($client_name);
  $scopes = openid_connect_get_scopes();

  // Custom code: Add custom scope.
  $scopes .= ' sp_custom/functions';
  // End custom code.

  $_SESSION['openid_connect_op'] = 'login';
  $client->authorize($scopes);
}

/**
  * Return list with Procurios functions.
  */
function procoid_get_function_list() {
  return array(
    25 => 'bestelpersoon',
    33 => 'activist',
    38 => 'CRM gebruiker',
    47 => 'personeelslid',
  );
}

/**
 * Alter openid_connect_admin_form.
 */
function procoid_alter_openid_connect_admin_form(&$form, &$form_state) {
  
  // Add submit function.
  $form['#submit'][] = 'procoid_alter_openid_connect_admin_form_submit';

  // Add role function role mapping form.
  $form['function_role_mapping'] = array(
    '#title' => t('Function role mapping'),
    '#type' => 'fieldset',
  );

  $form['function_role_mapping']['restrict'] = array(
    '#type' => 'checkboxes',
    '#options' => array('restrict' => 'Restrict login to users with assigned role(s)'),
    '#default_value' => empty(variable_get('procoid_mapped_role_only_login', FALSE)) ? array() : array('restrict' => 'restrict'),
  );

  $configurable_roles = procoid_get_configurable_roles();
  $function_list = procoid_get_function_list();
  $function_role_mapping = variable_get('procoid_function_role_mapping', array());

  foreach ($configurable_roles as $role_id => $role_name) {
    $form['function_role_mapping']['role_' . $role_id] = array(
      '#title' => 'Role: ' . $role_name,
      '#type' => 'checkboxes',
      '#multiple' => TRUE,
      '#options' => $function_list,
      '#default_value' => $function_role_mapping[$role_id],
    );
  }
}

/**
 * Store login roles on submission.
 */
function procoid_alter_openid_connect_admin_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  // Store restriced login.
  variable_set('procoid_mapped_role_only_login', empty($values['function_role_mapping']['restrict']['restrict']) ? FALSE : TRUE);

  // Store function role mapping.
  $mapping = array();
  if (!empty($values['function_role_mapping'])) {
    // Store function role mapping.
    foreach ($values['function_role_mapping'] as $form_key => $form_values) {
      if ($form_key !== 'restrict') {
        if (!empty($form_values)) {
          $role_id = substr_replace($form_key, '', 0, 5);
          $mapping[$role_id] = $form_values;
        }
      }
    }
  }
  variable_set('procoid_function_role_mapping', $mapping);
}

/**
 * Returns configurable roles.
 */
function procoid_get_configurable_roles() {
  $roles = user_roles(TRUE);
  // Remove administrator role.
  if (($key = array_search('administrator', $roles)) !== false) {
    unset($roles[$key]);
  }
  // Remove authenticated user role.
  if (($key = array_search('authenticated user', $roles)) !== false) {
    unset($roles[$key]);
  }
  return $roles;
}

/**
 * Returns managed roles.
 */
function procoid_get_managed_roles() {
  $managed_roles = array();
  $function_role_mapping = variable_get('procoid_function_role_mapping', array());
  $managed_role_ids = array_keys($function_role_mapping);
  $configurable_roles = procoid_get_configurable_roles();
  if (!empty($managed_role_ids)) {
    foreach ($managed_role_ids as $managed_role_id) {
      $managed_roles[$managed_role_id] = $configurable_roles[$managed_role_id];
    }
  }
  return $managed_roles;
}
