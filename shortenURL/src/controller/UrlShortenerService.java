package controller;

import repository.UrlRepository;
import utils.Base62Encoder;

import java.util.concurrent.atomic.AtomicLong;

public class UrlShortenerService {
	
	private final UrlRepository repository;
	private final String domain = "http://short.ly/";
	private final AtomicLong counter = new AtomicLong(1);   // 
	
	public UrlShortenerService(UrlRepository repository) {
		this.repository = repository;
	}
	
	public String shorten(String longUrl) {
		long id = counter.getAndIncrement();
		String shortCode = Base62Encoder.encode(id);
		
		System.out.println("UrlShortenerService :: shorten :: shortCode - "+ shortCode);
		String shortUrl = domain + shortCode;
		
		repository.save(shortCode, longUrl);
		
		return shortUrl;
		
	}
	
	public String retrieve(String shortUrl) {
		String shortCode = shortUrl.replace(domain, "");
		
		System.out.println("UrlShortenerService :: retrieve :: shortCode - "+ shortCode);
		return repository.getLongUrl(shortCode);
		
	}

}


/*
		AtomicLong counter = new AtomicLong(1);
		This creates an atomic long variable initialized to 1.
		
		Atomic means it supports thread-safe operations without explicit synchronization.
		
		So if multiple threads access or modify counter, its value will be updated safely.
		
		long id = counter.getAndIncrement();
		This gets the current value of counter (which initially is 1) and then increments it by 1 atomically.
		
		So id will be assigned 1 the first time this is called.
		
		Next call will get 2, then 3, and so on.
*/



	/*  import java.util.concurrent.atomic.AtomicLong;
		
		public class IdGenerator {
		    private final AtomicLong counter = new AtomicLong(1);
		
		    public long getNextId() {
		        return counter.getAndIncrement();
		    }
		
		    public static void main(String[] args) {
		        IdGenerator generator = new IdGenerator();
		
		        // Create multiple threads to simulate concurrent ID generation
		        Runnable task = () -> {
		            long id = generator.getNextId();
		            System.out.println("Generated ID: " + id);
		        };
		
		        // Starting 5 threads
		        for (int i = 0; i < 5; i++) {
		            new Thread(task).start();
		        }
		    }
		}
		
		
		output :  
		
		Generated ID: 1
		Generated ID: 2
		Generated ID: 3
		Generated ID: 4
		Generated ID: 5

 */