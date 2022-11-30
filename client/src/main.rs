
mod client;
use client::do_client;

fn main() {
	do_client().expect( "client failed" );
}
