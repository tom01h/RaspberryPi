#!/usr/bin/perl
 
require HTTP::Request;
require LWP::UserAgent;
 
use JSON;
 
$request = HTTP::Request->new(GET => 'http://192.168.0.15/');
$ua = LWP::UserAgent->new;
$response = $ua->request($request);
if ($response->is_success) {
        $data_ref = decode_json( $response->content );
        print "$data_ref->{'pressure'}\n";
}
