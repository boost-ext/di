import dagger.*;
import javax.inject.Inject;

interface I { public void dummy(); }; class Impl implements I { @Inject Impl() { } @Override public void dummy() { } }

@Module
class AppModule {
    @Provides I provideI(Impl impl) { return impl; }
}

@Component(modules = AppModule.class)
interface AppComponent {
    I build();
}

public class dagger2 {
    public static void main(String[] args) {
        I object = Dagger_AppComponent.create().build();
        object.dummy();
   }
}

