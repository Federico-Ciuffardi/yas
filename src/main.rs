use std::fs::File;
use std::io::BufReader;
use std::io::prelude::*;
use std::path::PathBuf;
use clap::{Parser, Args, Subcommand};
use anyhow::{Context, Result};

#[derive(Parser)]
#[clap(author, version, about, long_about = None)]
#[clap(propagate_version = true)]
struct Cli {
    /// turn debugging information on
    #[clap(short, long, parse(from_occurrences))]
    debug: usize,

    #[clap(subcommand)]
    subcommand: Option<SubCommands>,
}

#[derive(Args)]
struct Add {
    /// local yas repository to add files to 
    repo: String,
    /// The added files will be encrypted in the yas local and remote repositories
    #[clap(short, long)]
    recursive : bool,
    /// The added files will be encrypted in the yas local and remote repositories
    #[clap(short, long)]
    encrypt : bool,
    /// Files to add
    paths: Vec<std::path::PathBuf>,
}

#[derive(Subcommand)]
enum SubCommands {
    /// add a file to a local yas repository 
    Add(Add),
}

fn main() -> Result<()> {
    let cli = Cli::parse();

    // // You can see how many times a particular flag or argument occurred
    // // Note, only flags can have multiple occurrences
    // match cli.debug {
    //     0 => println!("Debug mode is off"),
    //     1 => println!("Debug mode is kind of on"),
    //     2 => println!("Debug mode is on"),
    //     _ => println!("Don't be crazy"),
    // }

    match &cli.subcommand {
        Some(SubCommands::Add(args)) => add(args)?,
        None => {}
    }
    Ok(())
}

fn add(args : &Add) -> Result<(), anyhow::Error> {
    println!("{} {} {:?}", args.repo, args.encrypt, args.paths);

    for path in &args.paths {
        let f = File::open(path)
              .with_context(|| format!("Could not open file `{}`", path.display()))?;
        let f = BufReader::new(f);

        // for line in f.lines() {
        //   println!("{}", line.unwrap());
        // }
    }
    Ok(())
}
