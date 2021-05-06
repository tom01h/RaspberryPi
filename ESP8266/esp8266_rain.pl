#!/usr/bin/perl
 
require HTTP::Request;
require LWP::UserAgent;
 
use JSON;
 
$ip = $ARGV[1];
if($ARGV[0] == 2){$ip = 'http://192.168.0.'.$ip.'/ini';}
else             {$ip = 'http://192.168.0.'.$ip.'/';}

$request = HTTP::Request->new(GET => $ip);
$ua = LWP::UserAgent->new;
$response = $ua->request($request);
if ($response->is_success) {
        $data_ref = decode_json( $response->content );
        if($ARGV[0] == 0){ print "$data_ref->{'3 hour'}\n";}
        if($ARGV[0] == 1){ print "$data_ref->{'1 hour'}\n";}
        if($ARGV[0] == 2){ print "$data_ref->{'5 min'}\n";}
}
