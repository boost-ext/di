import dagger.*;
import javax.inject.Inject;
import javax.inject.Named;

interface I { public void dummy(); }; class Impl implements I { @Inject Impl() { } @Override public void dummy() { } }

class C {
    @Inject C(@Named("i1") I i, @Named("i2") I ii) {}
};

@Module
class AppModule {
    @Provides @Named("i1") I provideI1(Impl impl) { return impl; }
    @Provides @Named("i2") I provideI2(Impl impl) { return impl; }
}

@Component(modules = AppModule.class)
interface AppComponent {
    C build();
}

public class dagger2 {
    public static void main(String[] args) {
        C object = Dagger_AppComponent.create().build();
        //object.dummy();
   }
}

