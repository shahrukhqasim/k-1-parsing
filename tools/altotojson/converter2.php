<?php

function getTags($string, $tagname) {
    // echo $string;
    $pattern = "/<$tagname.*?\/>/";
    if(preg_match_all($pattern, $string, $matches))
      echo "found\n";
    else
      echo "not found\n";

      echo ("Size is ".sizeof($matches[0])."\n");
      //echo $matches[0]."\n";
    return $matches[0];
}

$handle = fopen("files.txt", "r");

if ($handle) {
    while (($line = fgets($handle)) !== false) {
        $fileName=$line;
        $fileName=substr($fileName,0,strlen($fileName)-1);
        echo $fileName."\n";
        $path_parts = pathinfo($fileName);
        $outFile=$path_parts['dirname']."/".$path_parts['filename'].".json";
        echo $outFile."\n";
        $html = file_get_contents($fileName);
        echo $html;
        $json=element_to_obj($html);
        $withoutExt = preg_replace('/\\.[^.\\s]{3,4}$/', '', $fileName);
        echo "Wrinting ".$withoutExt.".json";
        //echo json_encode($json);
        file_put_contents($withoutExt.".json",$json);
    }

    fclose($handle);
} else {
    // error opening the file.
}

function createElementFromHTML($doc,$str) {
       $d = new DOMDocument();
       $d->loadHTML($str);
       return $doc->importNode($d->documentElement,true);
  }


function element_to_obj($element) {
  $json=array();
  // $data=new DOMDocument;
  // $data->loadHTML($element);
  // $words = $data->getElementsByTagName('TextLine');
  $matches=getTags($element,"String");


  foreach($matches as $x) {
    echo $x."\n";

    $ab=new DOMDocument;
    $ab->loadHTML($x);
        echo $ab->saveHtml();
    $ab=$ab->getElementsByTagName("string");

    //echo $a->saveHtml();
      foreach ($ab as $a) {
      // echo "hello2";
      // echo $data->saveHtml($word);
      $attrs = array();
      // // for ($i = 0; $i < $a->attributes->length; ++$i) {
      // //   $node = $a->attributes->item($i);
      // //   $attrs[$node->nodeName] = $node->nodeValue;
      // // }
      $reg['t']=intval($a->getAttribute('vpos'));
      $reg['l']=intval($a->getAttribute('hpos'));
      $reg['b']=intval($a->getAttribute('height'))+$reg['t'];
      $reg['r']=intval($a->getAttribute('width'))+$reg['l'];
      $attrs['Region']=$reg;
      $str=$a->getAttribute('content');

      $attrs['Value']=$str;

      // $w=array("x1"=>$x1,"x2"=>$x2,"y1"=>$y1,"y2"=>$y2);
      array_push($json,$attrs); }
  }
  $pages=array();
  $pages[0]["Words"]=$json;
  $every["Pages"]=$pages;

  return json_encode($every);
}

?>
