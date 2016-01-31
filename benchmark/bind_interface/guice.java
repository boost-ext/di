import com.google.inject.*;

interface I { public void dummy(); }; class Impl implements I { @Inject Impl() { } @Override public void dummy() { } }

class Module extends AbstractModule {
    @Override
    protected void configure() {
        bind(I.class).to(Impl.class);
    }
}

public class guice {
    public static void main(String[] args) {
        Injector injector = Guice.createInjector(new Module());        
        I object = injector.getInstance(I.class);
        object.dummy();
    }
}

