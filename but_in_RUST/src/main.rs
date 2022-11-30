
use clap::Parser;
mod http_server;

#[derive( Parser, Debug )]
#[command( name = "app",
    author = "me",
    version = "one",
    about = "this is desc",
    long_about = "this is long desc" )]

struct CliArgs {
    #[arg( default_value_t = 8000 )]
    port: u16,
}

fn main () {
    let cli = CliArgs::parse();

    println!( "Hello, world!" );
    http_server::start_server( cli.port );
}
