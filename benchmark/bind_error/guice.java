import com.google.inject.*;

interface I { public void dummy(); }; class Impl implements I { @Inject Impl() { } @Override public void dummy() { } }

class Module extends AbstractModule {
    @Override
    protected void configure() {
        //bind(I.class).to(Impl.class);
    }
}

public class guice {
    public static void main(String[] args) {
        Injector injector = Guice.createInjector(new Module());        
        I object = injector.getInstance(I.class);
        object.dummy();
    }
}

//Exception in thread "main" com.google.inject.ConfigurationException: Guice configuration errors:

//1) No implementation for I was bound.
  //while locating I

//1 error
	//at com.google.inject.internal.InjectorImpl.getProvider(InjectorImpl.java:1042)
	//at com.google.inject.internal.InjectorImpl.getProvider(InjectorImpl.java:1001)
	//at com.google.inject.internal.InjectorImpl.getInstance(InjectorImpl.java:1051)
	//at guice.main(guice.java:15)

