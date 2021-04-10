#!/usr/bin/perl
 
require HTTP::Request;
require LWP::UserAgent;
 
use JSON;
 
$ip = $ARGV[0];
$ip = 'http://192.168.0.'.$ip.'/';

$request = HTTP::Request->new(GET => $ip);
$ua = LWP::UserAgent->new;
$response = $ua->request($request);
if ($response->is_success) {
        $data_ref = decode_json( $response->content );
        print "$data_ref->{'pressure'}\n";
}
