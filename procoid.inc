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
  $scopes .= ' sp_custom/extra';
  // End custom code.

  $_SESSION['openid_connect_op'] = 'login';
  $client->authorize($scopes);
}

/**
  * Return list with Procurios functions.
  */
function procoid_get_function_list() {
  return array(
    33 => 'activist',
    34 => 'afdelingscoach',
    25 => 'bestelpersoon',
    6 => 'bestuurslid',
    8 => 'burgemeester',
    36 => 'contactpersoon ROOD',
    38 => 'CRM gebruiker',
    39 => 'Eerste Kamerlid',
    41 => 'Europees Parlementslid',
    31 => 'fractievoorzitter',
    18 => 'gedeputeerde',
    17 => 'hulpdienstmedewerker',
    7 => 'kaderlid',
    42 => 'lid beroepscommissie',
    43 => 'lid dagelijks bestuur',
    44 => 'lid financiële controlecommissie',
    45 => 'lid V-team',
    35 => 'lokale scholer',
    14 => 'medewerker scholingsteam',
    19 => 'parlementariër',
    46 => 'partijbestuurslid',
    3 => 'penningmeester',
    47 => 'personeelslid',
    20 => 'raadslid',
    30 => 'regiovertegenwoordiger',
    4 => 'secretaris',
    32 => 'stagiair',
    27 => 'statenlid',
    37 => 'tribune verantwoordelijke',
    40 => 'Tweede Kamerlid',
    28 => 'verantwoordelijke kieslijst',
    5 => 'vicevoorzitter',
    1 => 'voorzitter',
    23 => 'vrijwilliger',
    24 => 'waarnemer',
    29 => 'webmaster',
    13 => 'wethouder',
  );
}

/**
 * Alter openid_connect_admin_form.
 */
function procoid_alter_openid_connect_admin_form(&$form, &$form_state) {
  $configurable_roles = procoid_get_configurable_roles();

  $form['#prefix'] = "<div id='procoid_ajax'>";
  $form['#suffix'] = "</div>";
  
  // Add submit function.
  $form['#validate'][] = 'procoid_alter_openid_connect_admin_form_submit';

  // Add restrict login form.
  $form['role_restrict'] = array(
    '#type' => 'fieldset',
    '#title' => 'Restrict login',
  );

  $form['role_restrict']['restrict'] = array(
    '#type' => 'checkboxes',
    '#options' => array('restrict' => 'Only Procurios users with mapped role(s) can login with OpenID'),
    '#default_value' => empty(variable_get('procoid_mapped_role_only_login', FALSE)) ? array() : array('restrict' => 'restrict'),
  );

  // Add membership role mapping form.
  $form['membership_role_mapping'] = array(
    '#title' => t('Membership role mapping'),
    '#type' => 'fieldset',
  );
 
  $memberships = array('sp', 'red');
  foreach ($memberships as $membership) {
    $default_value = variable_get('procoid_' . $membership . '_membership_role', '');
    $form['membership_role_mapping'][$membership . '_membership_role'] = array(
      '#title' => t('@membership member role:', array('@membership' => strtoupper($membership))),
      '#type' => 'select',
      '#options' => $configurable_roles,
      '#empty_option' => ' - Choose - ',
      '#default_value' => $default_value,
      '#ajax' => array(
        'callback' => 'procoid_ajax_callback',
        'wrapper' => 'procoid_ajax',
      ),
    );
  }

  // Add role function role mapping form.
  foreach ($memberships as $membership) {
    $selected_membership_role = variable_get('procoid_' . $membership . '_membership_role', '');
    if (!empty($selected_membership_role)) {
      unset($configurable_roles[$selected_membership_role]);
    }
  }
  $form['function_role_mapping'] = array(
    '#title' => t('Function role mapping'),
    '#type' => 'fieldset',
  );

  $function_list = procoid_get_function_list();
  $function_role_mapping = variable_get('procoid_function_role_mapping', array());

  $selected_function_list = array();
  foreach ($configurable_roles as $role_id => $role_name) {
    $form['function_role_mapping']['role_' . $role_id] = array(
      '#title' => 'Role: ' . $role_name,
      '#type' => 'checkboxes',
      '#multiple' => TRUE,
      '#options' => $function_list,
      '#default_value' => empty($function_role_mapping[$role_id]) ? array() : $function_role_mapping[$role_id],
      '#ajax' => array(
        'callback' => 'procoid_ajax_callback',
        'wrapper' => 'procoid_ajax',
      ),
    );
    if (isset($function_role_mapping[$role_id])) {
      foreach ($function_role_mapping[$role_id] as $function_id => $function_value) {
        if (!empty($function_value)) {
          $selected_function_list[$function_id] = $function_list[$function_id];
        }
      }
    }
  }

  // Add Procurios selection select for cron sync.
  natsort($selected_function_list);
  $selected_functions = implode(', ', $selected_function_list);
  $form['procurios_import_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Procurios import settings',
    '#description' => t('Accounts will be imported from a Procurios selection using "drush procurios-import-users". This command should be executed regularly (cron) to keep account information up-to-date. If such a selection does not exist yet, then create a selection in Procurios containing contacts having one of these functions: @functions.', array('@functions' => $selected_functions)),
  );

  $selection_list = procapi_selection_get_select_list();
  $form['procurios_import_settings']['selection'] = array(
    '#type' => 'select',
    '#title' => t('Selection', array('@functions' => $selected_functions)),
    '#default_value' => variable_get('procoid_selection', array()),
    '#options' => $selection_list,
    '#empty_option' => '- kies -',
  );

  $user_field_list = procoid_get_user_field_list();
  if (!empty($user_field_list)) {
    $form['procurios_import_settings']['contactnumber_field'] = array(
      '#type' => 'select',
      '#title' => t('Choose the contactnumber field.'),
      '#description' => t('The contactnumber field will be used to match existing accounts with newly imported accounts from Procurios.'),
      '#default_value' => variable_get('procoid_contactnumber_field', array()),
      '#options' => $user_field_list,
      '#empty_option' => '- kies -',
    );
  }
}

/**
 * Store login roles on submission.
 */
function procoid_alter_openid_connect_admin_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  // Store restriced login.
  variable_set('procoid_mapped_role_only_login', empty($values['role_restrict']['restrict']['restrict']) ? FALSE : TRUE);

  // Store function role mapping.
  $mapping = array();
  if (!empty($values['function_role_mapping'])) {
    // Store function role mapping.
    foreach ($values['function_role_mapping'] as $form_key => $form_values) {
      if (!empty($form_values)) {
        $role_id = substr_replace($form_key, '', 0, 5);
        $mapping[$role_id] = $form_values;
      }
    }
  }
  variable_set('procoid_function_role_mapping', $mapping);
  variable_set('procoid_selection', $values['procurios_import_settings']['selection']);
  variable_set('procoid_contactnumber_field', $values['procurios_import_settings']['contactnumber_field']);
  variable_set('procoid_sp_membership_role', $values['membership_role_mapping']['sp_membership_role']);
  variable_set('procoid_red_membership_role', $values['membership_role_mapping']['red_membership_role']);
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

/**
 * Ajax callback.
 */
function procoid_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Return user fields.
 */
function procoid_get_user_field_list() {
  $fields_info = field_info_instances('user');
  if (!empty($fields_info['user'])) {
    foreach ($fields_info['user']  as $field_key => $field_info) {
      $field_list[$field_key] = $field_info['label'];
    }
  }
  return $field_list;
}

/**
 * Load user by registrationnumber.
 */
function procoid_user_load_by_registrationnumber($userinfo) {
  if (!empty($userinfo['sp_custom/extra']['contact_number'])) {
    // Check if an existing account can be found by comparing contact number.
    $contactnumber_field = variable_get('procoid_contactnumber_field');
    if (!empty($contactnumber_field)) {
      // Get account.
      $query = new EntityFieldQuery();
      $query
        ->entityCondition('entity_type', 'user')
        ->fieldCondition($contactnumber_field, 'value', $userinfo['sp_custom/extra']['contact_number'], '=');
      $result = $query->execute();
      if (!empty($result['user'])) {
        $userdata = array_pop($result['user']);
        $account = user_load($userdata->uid);

        // Update userinfo.
        $edit = array(
          'name' => openid_connect_generate_username($sub, $userinfo, 'procurios'),
          'pass' => user_password(),
          'mail' => $userinfo['email'],
          'init' => $userinfo['email'],
          'status' => 1,
          'openid_connect_client' => $client_name,
          'openid_connect_sub' => $sub,
        );
        $account = user_save($account, $edit);
        return $account;
      }
    }
  }
}

/**
 * Save userinfo.
 */
function procoid_save_userinfo($userinfo) {
  // Code mostly based on openid_connect_complete_authorization function.
  $sub = $userinfo['sub'];
  if (empty($sub)) {
    watchdog('procoid', 'No "sub" found from @provider', array('@provider' => 'procurios', WATCHDOG_ERROR));
    return FALSE;
  }

  $account = openid_connect_user_load_by_sub($sub, 'procurios');

  if (empty($account)) {
    $account = procoid_user_load_by_registrationnumber($userinfo);
  }


  if ($account) {
    // Connect account to procurios relation.
    openid_connect_connect_account($account, 'procurios', $sub);

    // An existing account was found. Save user claims.
    if (variable_get('openid_connect_always_save_userinfo', TRUE)) {
      openid_connect_save_userinfo($account, $userinfo);
    }
    $account_is_new = FALSE;
  }
  else {
    // Check whether the e-mail address is valid.
    if (!filter_var($userinfo['email'], FILTER_VALIDATE_EMAIL)) {
      drupal_set_message(t('The e-mail address %mail is not valid.', array('%mail' => $userinfo['email'])), 'error');

      return FALSE;
    }
    // Check whether there is an e-mail address conflict.
    if (user_load_by_mail($userinfo['email'])) {
      drupal_set_message(t('The e-mail address %email is already taken!.', array('%email' => $userinfo['email'])), 'error');

      return FALSE;
    }

    // Create a new account.
    $account = openid_connect_create_user($sub, $userinfo, 'procurios');
    // Reload $account in case it has been altered in a user hook elsewhere.
    $account = user_load($account->uid);
    openid_connect_save_userinfo($account, $userinfo);
    $account_is_new = TRUE;
  }

  module_invoke_all('openid_connect_post_authorize', NULL, $account, $userinfo, 'procurios', $account_is_new);
  return $account->uid;
}

/**
 * Get mapped roles.
 */
function procoid_get_mapped_roles() {
  $mapped_roles = array();
  $function_role_mapping = variable_get('procoid_function_role_mapping', array());
  foreach ($function_role_mapping as $role_id => $functions) {
    foreach ($functions as $function_id => $state) {
      if (!empty($state)) {
        $mapped_roles[$role_id] = $role_id;
        break 2;
      }
    }
  }
  return $mapped_roles;
}

/**
 * Get users by role.
 */
function procoid_get_users_by_role($rid = 1) { // rid = Role Id from users_roles table
    $query = db_select('users', 'u');
    $query->fields('u', array('uid', 'name'));
    $query->innerJoin('users_roles', 'r', 'r.uid = u.uid');
    $query->condition('r.rid', $rid);
    $query->orderBy('u.name');
    $result = $query->execute();
    $users = array();
    foreach ($result as $user) {
        $users[] = $user;
    }
    return $users;
}
