<?php
$api="";
$username = $_GET['username'];
$save_name = $_GET['save'];
$text = $_GET['text'];
$url = "https://api.telegram.org/bot$api/getupdates";
$ch = curl_init();
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_URL, $url);
$result = curl_exec($ch);
$update = json_decode($result);
$mm = $update->result;
foreach($mm as $msg){
    if($msg->message->from->username == $username){
        $id = $msg->message->from->id;
        $chh = "";
        if($text=="JW"){$chh= "JOHN WATSON";}
        if($text=="JS"){$chh= "JOHN SMITH";}
        if($text=="JB"){$chh= "JERMY BERT";}
        if($text=="MS"){$chh= "MISS STEALTHY";}
        if($text=="WG"){$chh= "WILLIAM GULL";}
        if($text=="SG"){$chh= "SERGENT GOODLEY";}
        if($text=="SH"){$chh= "SHERLOCK HOLMES";}
        if($text=="IL"){$chh= "INSPECTOR LESTRADE";}
        
        $ttxxt = urlencode("Hello!🙂\n🎩🕵️‍♀️The Jack in `$save_name` save is : `".$chh."` !");
        $url2 = "https://api.telegram.org/bot".$api."/sendMessage?chat_id=".$id."&text=".$ttxxt ."&parse_mode=markdown";
        curl_setopt($ch, CURLOPT_URL, $url2);
        $result = curl_exec($ch);
        if(json_decode($result)->ok)
           echo "Success!";
        exit(0);
    }
}
?>