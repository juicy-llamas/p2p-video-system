
use std::net::{ SocketAddr, TcpListener, TcpStream };
use std::io::prelude::*;
use std::thread;
use std::thread::JoinHandle;

pub fn send_stuff ( mut stream: TcpStream ) {
// 	loop {
	let mut res: std::io::Result<usize>;
	let mut buffer: &mut [u8] = &mut [ 0; 4096 ];

	loop {
		res = stream.read( &mut buffer );
		match res {
			Ok( 0 ) => { println!( "none" ); return; },
			Err( e ) => { println!( "{}", e ); return; },
			Ok( _ ) => {
				let read_bytes = String::from_utf8( buffer.to_vec() ).unwrap();
				println!( "{}", read_bytes );

				let content = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\" /><body><p>hello</p></body></html>";
				let mut header = String::from( "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nServer: I_Suck\r\nContent-Length: \r\n\r\n" );
				{
					let len_s = content.len().to_string();
					header.insert_str( 74, len_s.as_str() );
					header.push_str( content );
				}
				println!( "{}", header );
				stream.write( header.as_bytes() ).expect( "failed to write" );
			}
		}
	}
}

pub fn start_server ( port: u16 ) {
	let addr = SocketAddr::from( ( [0,0,0,0], port ) );
	let mut hands: Vec<JoinHandle<_>> = Vec::with_capacity( 16 );
	let receiver = TcpListener::bind( addr )
		.expect( "couldn't bind");

	println!( "Hello, TCP!" );

	for i in receiver.incoming() {
		match i {
			Ok( stream ) => {
				let hand = thread::spawn( || {
					println!( "{}", stream.peer_addr().unwrap() );
					stream.set_nonblocking( false ).unwrap();
					send_stuff( stream );
				} );
				hands.push( hand );
			},
			Err( e ) => println!( "{}", e )
		}
	}

	for hand in hands.into_iter() {
		hand.join();
	}
}
