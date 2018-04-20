open Jest;

type t = {clientId: int};

external castCompleteToJson : Express.complete => Js.Json.t = "%identity";

external createJson : t => Js.Json.t = "%identity";

describe("Awesomize Express", () =>
  describe("make", () => {
    let schema = [|
      (
        "clientId",
        {
          read: Awesomize.Read.key("clientId"),
          sanitize: None,
          validate: [Awesomize.Validator.required],
          normalize: None,
        }: Awesomize.definition,
      ),
    |];
    let decoder = (_) => ();
    let encoder = (_) => createJson([%raw "{clientId: 1}"]);
    let handler = (req, _) => Js.Promise.resolve();
    let res: Express.Response.t = [%raw
      "{ status: (code) => ({ json: () => ({clientId: code === 200 ? 1 : -1}) }) }"
    ];
    let convertJsonToT = complete => {
      clientId:
        Json.Decode.field(
          "clientId",
          Json.Decode.int,
          castCompleteToJson(complete),
        ),
    };
    testPromise("success", () => {
      let req: Express.Request.t = [%raw "{clientId: 1}"];
      AwesomizeExpress.make(
        ~schema,
        ~decoder,
        ~encoder,
        ~handler,
        (),
        req,
        res,
      )
      |> Js.Promise.then_(result =>
           (
             switch (convertJsonToT @@ result) {
             | {clientId: 1} => pass
             | _ => fail("not an expected result")
             }
           )
           |> Js.Promise.resolve
         );
    });
    testPromise("fail", () => {
      let req: Express.Request.t = [%raw "{djfalsfjdskalfjs: 1}"];
      AwesomizeExpress.make(
        ~schema,
        ~decoder,
        ~encoder,
        ~handler,
        (),
        req,
        res,
      )
      |> Js.Promise.then_(result =>
           (
             switch (convertJsonToT @@ result) {
             | {clientId: (-1)} => pass
             | _ => fail("not an expected result")
             }
           )
           |> Js.Promise.resolve
         );
    });
  })
);
