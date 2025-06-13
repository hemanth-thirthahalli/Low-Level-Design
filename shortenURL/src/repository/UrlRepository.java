package repository;

public interface UrlRepository {
	void save(String shortUrl, String longUrl);
	String getLongUrl(String shortUrl);
}
