package repository;
import java.util.concurrent.ConcurrentHashMap;
import java.lang.*;

public class InMemoryUrlRepository implements UrlRepository{
	private final ConcurrentHashMap<String, String> storage = new ConcurrentHashMap<>();
	
	@Override
	public void save(String shortUrl, String longUrl) {
		storage.put(shortUrl, longUrl);
	}
	
	@Override
	public String getLongUrl(String shortUrl) {
		return storage.get(shortUrl);
	}
	
}
