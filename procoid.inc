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
  // Add submit function.  $form['#submit'-
  $form['#submit'][] = 'procoid_alter_openid_connect_admin_form_submit';
  $form['function_role_mapping'] = array(
    '#title' => t('Function role mapping'),
    '#type' => 'fieldset',
  );

  $roles = user_roles(TRUE);
  // Remove administrator role.
  if (($key = array_search('administrator', $roles)) !== false) {
    unset($roles[$key]);
  }
  // Remove authenticated user role.
  if (($key = array_search('authenticated user', $roles)) !== false) {
    unset($roles[$key]);
  }

  $function_list = procoid_get_function_list();
  $function_role_mapping = variable_get('procoid_function_role_mapping', array());

  foreach ($roles as $role_id => $role_name) {
    $form['function_role_mapping']['role_' . $role_id] = array(
      '#title' => 'Role: ' . $role_name . '<br/>Function:',
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $function_list,
      '#empty_option' => ' - Maak een keuze - ',
      '#default_value' => $function_role_mapping[$role_id],
    );
  }
}

/**
 * Adds function role mapping to openid connect admin form.
 */
function procoid_alter_openid_connect_admin_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $mapping = array();
  if (!empty($values['function_role_mapping'])) {
    // Store function role mapping.
    foreach ($values['function_role_mapping'] as $form_key => $form_values) {
      $role_id = substr_replace($form_key, '', 0, 5);
      $mapping[$role_id] = $form_values;
    }
  }
  variable_set('procoid_function_role_mapping', $mapping);
}
