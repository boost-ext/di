import com.google.inject.*;

class Module extends AbstractModule {
    @Override
    protected void configure() {
        bind(int.class).toInstance(42);
    }
}

public class guice {
    public static void main(String[] args) {
        Injector injector = Guice.createInjector(new Module());        
        System.exit(injector.getInstance(int.class) != 42 ? 1 : 0);
    }
}

