<?php

class IslandoraSolrSortOptions {
  
  function getAvailableSorts() {
    return array(
      'date' => array(
        'title' => 'Date',
        'default' => 'desc',        
      ),
      'score' => array(
        'title' => 'Relevance',
        'default' => 'desc',
      ),
    );
  }
  
  /**
   * Function: displaySortOptions
   *
   * Description: Displays sort options based on an apache solr query response,
   *   as contained with the IslandoraSolrQueryProcessor.
   *
   * @global string $base_url
   * @param IslandoraSolrQueryProcessor $solrQueryProcessor
   * @return string
   */
  function displaySortOptions($solrQueryProcessor) {
    
    GLOBAL $base_url;
    
    $query = $solrQueryProcessor->solrQuery;
    
    if (empty($query)) {
      return '';
    }
    
    $path = $base_url . parse_url(request_uri(), PHP_URL_PATH); // TODO accommodate subqueries
    $sorts = $this->getAvailableSorts();
    $current_sort = $solrQueryProcessor->solrParams['sort'];
    list($current_sort_by,$current_sort_order) = explode(' ',$current_sort);
    
    if(empty($current_sort_by)) {
      $current_sort_by = 'score';
    }
    if(empty($current_sort_order)) {
      $current_sort_order = $sorts[$current_sort_by]['default'];
    }
    
    $toggle = array('asc' => 'desc', 'desc' => 'asc');
    $sort_links = array();
    foreach ($sorts as $name => $sort) {
      $new_query = clone $solrQueryProcessor;
      $active = ($current_sort_by == $name);
      if ($active) {
        if ($name == 'score') {
          $direction = '';
          $new_direction = '';
        }
        else {
          $direction = $toggle[$current_sort_order];
          $new_direction = $toggle[$current_sort_order];
        }
      }
      else {
        $new_direction = $sort['default'];
      }
      
      $new_query->solrParams['sort'] = trim($name ." ". $new_direction);
      $str = (trim($name ." ". $new_direction));
      var_dump(_islandora_solr_sort_get_query_values($new_query));
      
      $sort_links[$name] = array(
        'title' => $sort['title'],
        'path' => $path,
        'options' => array(
          'query' => _islandora_solr_sort_get_query_values($new_query),
        ),
        'active' => $active,
        'direction' => $new_direction, // what happens to new_order???
      );    
    }
    if (!empty($sort_links)) {
      foreach ($sort_links as $name => $link) {
        $themed_links[$name] = islandora_solr_sort_output_link($link['title'], $link['path'], $link['options'], $link['active'], $link['direction'], $solrQueryProcessor);
      }
      return 'Sort by: '. theme('item_list',$themed_links);
    }
    else {
      return '';
    }
  }
}