<?php

function procoid_views_data() {
  $data['procoid_function']['table']['group'] = t('Function');

  $data['procoid_function']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Functions'),
    'weight' => -10,
  );

  $data['procoid_function']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['procoid_function']['uid'] = array(
    'title' => t('User id'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
      'title' => t('Add user relationship'),
      'help' => t('More information on this relationship'),
    ),
  );

  $data['procoid_function']['fid'] = array(
    'title' => t('Function id'),
    'help' => t('Procurios function id'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['procoid_function']['function'] = array(
    'title' => t('Function name'),
    'help' => t('Procurios function name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['procoid_function']['did'] = array(
    'title' => t('Department id'),
    'help' => t('Procurios department id'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['procoid_function']['department'] = array(
    'title' => t('Department name'),
    'help' => t('Procurios department name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}
