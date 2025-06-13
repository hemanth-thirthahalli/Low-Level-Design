package application;

import controller.UrlShortenerService;
import repository.InMemoryUrlRepository;


public class Main {

	public static void main(String[] args) {
		UrlShortenerService service = new UrlShortenerService(new InMemoryUrlRepository());
		
		String longUrl = "https://www.example.com/some/very/long/url";
        String shortUrl = service.shorten(longUrl);
        
        System.out.println("Short URL: " + shortUrl);
        System.out.println("Original URL: " + service.retrieve(shortUrl));


	}

}

